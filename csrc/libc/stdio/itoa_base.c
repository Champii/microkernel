/*
 * File: itoa_base.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: convert a number to a string
 *
 */

#include "itoa_base.h"

static char ref[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static size_t reverse_string(char* dst, const char* src, int cpt)
{
  int i = 0;

  for (cpt--; cpt >= 0; cpt--, i++)
    dst[i] = src[cpt];

  dst[i] = 0;

  return i;
}

size_t itoa_base(int n, char *s, int base)
{
  unsigned int cpt = 0;
  char neg = 0;
  char buff[32];

  if (n == 0)
    buff[cpt++] = '0';

  if (n < 0)
  {
    neg = 1;
    n *= -1;
  }

  while (n > 0)
  {
    buff[cpt++] = ref[n % base];
    n /= base;
  }

  if (neg)
  {
    *s = '-';
    s++;

    return reverse_string(s, buff, cpt) + 1;
  }

  return reverse_string(s, buff, cpt);
}

size_t uitoa_base(unsigned int n, char *s, int base)
{
  unsigned int cpt = 0;
  char buff[32];

  if (n == 0)
    buff[cpt++] = '0';

  while (n > 0)
  {
    buff[cpt++] = ref[n % base];
    n /= base;
  }

  return reverse_string(s, buff, cpt);
}
