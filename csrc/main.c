/************************************************
*
* Micro Kernel
*
* - Main C entry point
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include      "screen.h"

void          main()
{
  int         color;

  color = COLOR_WHITE;

  printk(color, "Hello World !\n");

  for (;;);
}
