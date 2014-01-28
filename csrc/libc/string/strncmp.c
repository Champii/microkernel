/*
 * File: strncmp.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: string comparison
 *
 */

#include <string.h>

int strncmp(const char* a, const char* b, size_t len)
{
  size_t i = 0;

  for ( ; a[i] && b[i] && len; i++, len--)
    if (a[i] != b[i])
      return b[i] - a[i];

  if (len == 0)
    return 0;

  return b[i] - a[i];
}
