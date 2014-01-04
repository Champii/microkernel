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
#include      "gdt.h"

int           main()
{
  int         color;


  init_gdt();

  color = COLOR_WHITE;

  clear_screen();

  printk(color, "Hello World !\n");

  for (;;);

  return 0;
}
