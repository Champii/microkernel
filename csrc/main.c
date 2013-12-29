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

int           main()
{
  int         color;

  color = COLOR_WHITE;

  clear_screen();

  printk(color, "Hello World !\n");

  for (;;);

  return 0;
}
