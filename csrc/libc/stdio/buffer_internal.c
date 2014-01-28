/*
 * File: buffer_internal.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: simple buffer implemenation for printf
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include "buffer_internal.h"

s_buff *make_buff(size_t max)
{
  s_buff *b = NULL;

  b = malloc(sizeof (s_buff));
  if (!b)
    return NULL;

  b->size = 0;
  b->max = max;
  b->tab = malloc(sizeof (char) * max);
  if (!b->tab)
  {
    free(b);
    return NULL;
  }

  return b;
}

void reset_buff(s_buff *b)
{
  b->size = 0;
}

void free_buff(s_buff *b)
{
  free(b->tab);
  free(b);
}

void add_string(s_buff *b, const char *str, size_t l)
{
  if (str == NULL)
    return;

  for (size_t i = 0; i < l; i++)
    add_char(b, str[i]);
}

void add_char(s_buff *b, char c)
{
  if (b->size == b->max - 1)
  {
    b->max *= 2;
    b->tab = realloc(b->tab, sizeof(char) * b->max);
    if (!b->tab)
    {
      write(BW_COLOR, "Internal buffer: cannot realloc", 31);
      return;
    }
  }
  b->tab[b->size++] = c;
  b->tab[b->size] = '\0';
}

char *get_string(s_buff *b)
{
  return b->tab;
}

size_t get_size(s_buff *b)
{
  return b->size;
}
