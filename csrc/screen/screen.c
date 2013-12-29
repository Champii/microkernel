/************************************************
*
* Micro Kernel
*
* - Standard printing funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include      "screen.h"

void          print()
{
  unsigned char        *ptr;
  int         attrib = (0 << 4) | (15 & 0x0F);
  unsigned att = attrib << 8;

  ptr = (unsigned char *)SCREEN_PTR;
  *ptr = 'a' | att;
}
