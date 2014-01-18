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
  init_gdt();

  clear_screen();

  printk(COLOR_RED, "Hello World !\n");

  for (;;);

  return 0;
}
