/*
 * File: strcmp.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: string comparison
 *
 */

#include <string.h>

int strcmp(const char* a, const char* b)
{
  size_t i = 0;

  for ( ; a[i] && b[i]; i++)
    if (a[i] != b[i])
      return b[i] - a[i];

  return b[i] - a[i];
}
