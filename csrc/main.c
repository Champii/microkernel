/************************************************
*
* Micro Kernel
*
* - Main C entry point
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/
#include      "system.h"
#include      "screen.h"
#include      "gdt.h"
#include      "idt.h"

int           main()
{
  int         color;

  init_gdt();
  init_idt();

  color = COLOR_WHITE;

  clear_screen();
  
  printk(color, "Hello World !\n");
  printk(color, 19/0);

  for (;;);

  return 0;
}
