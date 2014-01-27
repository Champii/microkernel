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

  init_mm();
  printk(COLOR_CYAN, "-- MM LOADED -- !\n");

  init_gdt();
  printk(COLOR_CYAN, "-- GDT LOADED -- !\n");

  init_idt();
  printk(COLOR_CYAN, "-- IDT LOADED -- !\n");

  init_isrs();
  printk(COLOR_CYAN, "-- ISRS LOADED -- !\n");
}

int           main(void *mboot, int magic)
{
  // int         *test;
  // int         test2;

  init();

  printk(COLOR_WHITE, my_putnbr_base(magic, "0123456789ABCDEF"));

  printk(COLOR_RED, "\nHello World !\n");


  // Page Fault Test
  // test = 0xA0000000;
  // test2 = *test;

  // *test = 1;

  // Div by 0 Fault Test
  // printk(COLOR_RED, 19/0);

  for (;;);

  return 0;
}
