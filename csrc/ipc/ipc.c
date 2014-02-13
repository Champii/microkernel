/************************************************
*
* Micro Kernel
*
* - Ipc funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "ipc.h"
#include                  "screen.h"
#include                  "process.h"
#include                  "task.h"
#include                  "mm.h"

extern t_task             *current_task;


void                      send(u64 pid, void *msg, unsigned size)
{

  unsigned                *pid_split = (unsigned *)&pid;

  printk(COLOR_WHITE, "FROM = ");
  printk(COLOR_WHITE, my_putnbr_base(getpid(), "0123456789"));
  printk(COLOR_WHITE, "\nSEND : 0x");
  printk(COLOR_WHITE, my_putnbr_base(pid_split[0], "0123456789ABCDEF"));
  printk(COLOR_WHITE, " Task = 0x");
  printk(COLOR_WHITE, my_putnbr_base(pid_split[1], "0123456789ABCDEF"));
  printk(COLOR_WHITE, " msg = 0x");
  printk(COLOR_WHITE, my_putnbr_base(msg, "0123456789ABCDEF"));
  printk(COLOR_WHITE, " = ");
  printk(COLOR_WHITE, (char *)msg);
  printk(COLOR_WHITE, " size = ");
  printk(COLOR_WHITE, my_putnbr_base(size, "0123456789"));
  printk(COLOR_WHITE, "\n");

}

void                      recv(u64 pid, void *pool, unsigned pool_size)
{
  // switch_page_directory(current_task->page_directory);
  // unsigned                *pid_split = (unsigned *)&pid;

  // printk(COLOR_WHITE, "RECV : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(pid_split[0], "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " Task = 0x");
  // printk(COLOR_WHITE, my_putnbr_base(pid_split[1], "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " msg = 0x");
  // printk(COLOR_WHITE, my_putnbr_base(pool, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " size = ");
  // printk(COLOR_WHITE, my_putnbr_base(pool_size, "0123456789"));
  // printk(COLOR_WHITE, "\n");

}

