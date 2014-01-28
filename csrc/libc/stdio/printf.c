/*
 * File: printf.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: minimal printf implementation
 *
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "buffer_internal.h"
#include "itoa_base.h"

#define BUFF_SIZE 1024
#define ITOA_BUFF_SIZE 128

static void handle_int(s_buff *buff, char mode, int n)
{
  char s[ITOA_BUFF_SIZE];
  size_t len = 0;

  if (mode == 'd')
    len = itoa_base(n, s, 10);
  else if (mode == 'u')
    len = uitoa_base((unsigned int)n, s, 10);
  else if (mode == 'x')
    len = uitoa_base((unsigned int)n, s, 16);
  else if (mode == 'o')
    len = itoa_base(n, s, 8);
  else
    return;

  add_string(buff, s, len);
}

static void handle_string(s_buff *buff, va_list* args, char mode)
{
  if (mode == 's')
  {
    char* p = va_arg(*args, char*);

    if (p == NULL)
      add_string(buff, "(null)", 6);
    else
      add_string(buff, p, strlen(p));
    return;
  }
  if (mode == 'c')
  {
    /* Must be int here */
    char c = va_arg(*args, int);
    add_char(buff, c);
    return;
  }
}

static void replace_args(s_buff *buff, va_list* args, char mode)
{
  if (mode == '%')
  {
    add_char(buff, '%');
    return;
  }
  if (mode == 's' || mode == 'c')
  {
    handle_string(buff, args, mode);
    return;
  }
  if (mode == 'd' || mode == 'o' || mode == 'x' || mode == 'u')
  {
    int n = va_arg(*args, int);
    handle_int(buff, mode, n);
    return;
  }
  else
  {
    add_char(buff, '%');
    add_char(buff, mode);
  }
}

static void parse_string(const char *format, s_buff* buff, va_list* args)
{
  for (int i = 0; format[i]; i++)
  {
    if (format[i] == '%')
      replace_args(buff, args, format[++i]);
    else
      add_char(buff, format[i]);
  }
}

int printf(const char *format, ...)
{
  int     res = 0;
  va_list args;
  s_buff* buff;

  buff = make_buff(BUFF_SIZE);
  if (!buff)
  {
    write(BW_COLOR, "Printf: cannot allocate a new buffer", 36);
    return -ENOMEM;
  }

  va_start(args, format);
  parse_string(format, buff, &args);
  res = get_size(buff);
  write(BW_COLOR, get_string(buff), res);
  free_buff(buff);

  va_end(args);

  return res;
}
