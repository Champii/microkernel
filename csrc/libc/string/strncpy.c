/*
 * File: strncpy.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: String copy
 *
 */

#include <string.h>

char* strncpy(char *s1, const char *s2, size_t n)
{
  for (; *s2 && n; n--, s1++, s2++)
    *s1 = *s2;

  for (; n; n--, s1++)
    *s1 = 0;

  return s1;
}
