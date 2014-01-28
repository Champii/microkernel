/*
 * File: memcpy.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: copy data to one memory location to another
 *
 */

#include <string.h>

void* memcpy(void *s1, const void *s2, size_t n)
{
  char* s1c = (char*)s1;
  char* s2c = (char*)s2;

  for ( ; n; s1c++, s2c++, n--)
    s1c[0] = s2c[0];

  return s1;
}
