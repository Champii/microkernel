/*
 * File: strlen.c
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: find length of string
 *
 */

#include <string.h>

size_t strlen(const char* s)
{
  size_t res = 0;

  for (; s[res]; res++)
    ;

  return res;
}
