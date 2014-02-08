/************************************************
*
* Micro Kernel
*
* - Main C entry point
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include      "multiboot.h"
#include      "system.h"
#include      "screen.h"
#include      "gdt.h"
#include      "mm.h"
#include      "idt.h"
#include      "isrs.h"
#include      "pic.h"
#include      "pit.h"
#include      "task.h"
#include      "syscall.h"
#include      "keyboard.h"
#include      "services.h"
#include      "syscall.h"

unsigned      initial_esp;

void          init(unsigned long multiboot_addr)
{
  struct s_multiboot_info *info;
  info = (struct s_multiboot_info *) multiboot_addr;

  clear_screen();

  init_idt();
  printk(COLOR_CYAN, "-- IDT && ISRS LOADED -- !\n");

  init_mm();
  printk(COLOR_CYAN, "-- MM LOADED -- !\n");

  init_gdt();
  printk(COLOR_CYAN, "-- GDT LOADED -- !\n");

  asm volatile("sti");
  printk(COLOR_CYAN, "-- Enabled interupts -- !\n");

  init_pit(100);
  printk(COLOR_CYAN, "-- PIT (Timer) LOADED -- !\n");

  init_syscalls();
  printk(COLOR_CYAN, "-- Syscalls LOADED -- !\n");

  init_tasking();
  printk(COLOR_CYAN, "-- Tasking LOADED -- !\n");

  init_services(info->mods_count, (struct s_multiboot_module *) info->mods_addr);
  printk(COLOR_CYAN, "-- Services LOADED -- !\n");

  init_keyboard();
  printk(COLOR_CYAN, "-- Keyboard LOADED -- !\n");


}

int           cmain(unsigned long magic, unsigned long addr, unsigned start_stack)
{

  if (magic != 0x2BADB002)
  {
    printk(COLOR_RED, "Bad Magic, System halted.\n");
    for (;;);
  }

  if (addr & 7)
  {
    printk(COLOR_RED, "Unaligned mbi = ");
    printk(COLOR_RED, my_putnbr_base(addr, "0123456789ABCDEF"));
    printk(COLOR_RED, "\n");
    return -1;
  }

  initial_esp = start_stack;


  init(addr);

  printk(COLOR_RED, "Hello World ! From : ");
  printk(COLOR_RED, my_putnbr_base(getpid(), "0123456789"));
  printk(COLOR_RED, "\n");

  for (;;);

  return 0;
}
