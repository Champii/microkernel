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
#include      "keyboard.h"


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

  init_keyboard();
  printk(COLOR_CYAN, "-- Keyboard LOADED -- !\n");

}

int           main(unsigned long magic, unsigned long addr)
{
  struct s_multiboot_tag *tag;
  unsigned    size;

  if (magic != 0x1BADB002)
  {
    printk(COLOR_RED, "Bad Magic, System halted.");
    for (;;);
  }

  size = *(unsigned *) addr;
  printk(COLOR_WHITE, "Multiboot header size : ");
  printk(COLOR_WHITE, my_putnbr_base(size, "01234564789"));
  printk(COLOR_WHITE, "\n");

  init();

  // printk(COLOR_WHITE, my_putnbr_base(magic, "0123456789ABCDEF"));

  printk(COLOR_RED, "\nHello World !\n");

  for (;;);

  return 0;
}
