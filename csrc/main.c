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
#include      "mm.h"
#include      "idt.h"
#include      "isrs.h"


void          init()
{
  clear_screen();

  init_gdt();
  printk(COLOR_CYAN, "-- GDT LOADED -- !\n");

  init_mm();
  printk(COLOR_CYAN, "-- MM LOADED -- !\n");

  init_idt();
  printk(COLOR_CYAN, "-- IDT LOADED -- !\n");

  init_isrs();
  printk(COLOR_CYAN, "-- ISRS LOADED -- !\n");
}

int           main()
{

  init();

  printk(COLOR_RED, "Hello World !\n");

  printk(COLOR_RED, 19/0);

  for (;;);

  return 0;
}
