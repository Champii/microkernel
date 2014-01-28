/*
 * File: memset.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: Set memory area to one specified value
 *
 */

#include <string.h>

void* memset(void* s1, int c, size_t n)
{
  char* s1c = (char*)s1;

  for ( ; n; s1c++, n--)
    s1c[0] = (unsigned char)c;

  return s1;
}
