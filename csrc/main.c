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
#include      "isrs.h"


int           main()
{
  int         color;

  init_gdt();
  printk(COLOR_CYAN, "-- GDT LOADED -- !\n");

  init_idt();
  printk(COLOR_CYAN, "-- IDT LOADED -- !\n");

  init_isrs();
  printk(COLOR_CYAN, "-- ISRS LOADED -- !\n");

  color = COLOR_WHITE;

  clear_screen();
  
  printk(color, "Hello World !\n");
  printk(color, 19/0);

  for (;;);

  return 0;
}
