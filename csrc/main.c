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
#include      "pic.h"
#include      "pit.h"


void          init()
{
  clear_screen();

  init_idt();
  printk(COLOR_CYAN, "-- IDT LOADED -- !\n");

  printk(COLOR_CYAN, "-- ISRS LOADED -- !\n");

  init_mm();
  printk(COLOR_CYAN, "-- MM LOADED -- !\n");

  init_gdt();
  printk(COLOR_CYAN, "-- GDT LOADED -- !\n");

  asm volatile("sti");
  printk(COLOR_CYAN, "-- Enabled interupts -- !\n");

  init_pit(100);
  printk(COLOR_CYAN, "-- PIT (Timer) LOADED -- !\n");

}

int           main()
{

  init();

  // printk(COLOR_WHITE, my_putnbr_base(magic, "0123456789ABCDEF"));

  printk(COLOR_RED, "\nHello World !\n");

  for (;;);

  return 0;
}
