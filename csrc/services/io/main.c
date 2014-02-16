#include <rpc/rpc.h>
#include <string.h>
#include <unistd.h>

unsigned itoa_base(int n, char *s, int base);

#include "screen.h"

void print_error(int ret)
{
  printk(COLOR_RED, "Error : ");
  switch (ret)
  {
    case -1:
      printk(COLOR_RED, "No more memory\n");
      break;
    case -2:
      printk(COLOR_RED, "Operation not permitted\n");
      break;
    case -3:
      printk(COLOR_RED, "Invalid argument\n");
      break;
    case -4:
      printk(COLOR_RED, "No such process\n");
      break;
    case -5:
      printk(COLOR_RED, "Bad address\n");
      break;
    case -6:
      printk(COLOR_RED, "Bad message\n");
      break;
    case -7:
      printk(COLOR_RED, "Message too long\n");
      break;

  }
}

void write_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  char tmp[10];
  printk(COLOR_WHITE, "WRITE RPC !\n");
  sender = sender;
  unsigned *uparams = (unsigned *)params;

  unsigned color = *uparams;
  uparams += sizeof(u32);
  char *str = (char *)uparams;
  uparams += sizeof(u32);
  unsigned size = *uparams;

  color = color;
  size = size;

  printk(15, str);

  itoa_base(color, tmp, 10);
  printk(COLOR_WHITE, "STR = ");
  printk(COLOR_WHITE, str);
  printk(COLOR_WHITE, " color = ");
  printk(COLOR_WHITE, tmp);
  itoa_base(size, tmp, 10);
  printk(COLOR_WHITE, " size = ");
  printk(COLOR_WHITE, tmp);
  printk(COLOR_WHITE, "\n");

  // params = params;
  *ret = 0;
  *ret_size = 4;
}

void welcome_screen()
{
  clear_screen();

  printk(COLOR_BLUE, "*******************************************************************************\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*                                 Micro Kernel                                *\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*                             Welcome to UserLand                             *\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*******************************************************************************\n");
  printk(COLOR_WHITE, "\n");
}

int main()
{

  welcome_screen();

  printk(COLOR_WHITE, "Starting IO service\n");


  struct rpc rpcs[4];
  rpcs[0].func_desc = "iisi";
  rpcs[0].handler = &write_rpc;
  rpcs[1].func_desc = "iiii";
  rpcs[1].handler = &write_rpc;
  rpcs[2].func_desc = "iiii";
  rpcs[2].handler = &write_rpc;
  rpcs[3].func_desc = "iiii";
  rpcs[3].handler = &write_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, 4)) < 0)
  {
    printk(COLOR_WHITE, "Error Register RPC\n");
    print_error(ret);
  }

  // sys_send(prog_loader_pid, "lol", 4);
  if ((ret = listen_rpc()) < 0)
  {
    printk(COLOR_WHITE, "Error Listen rpc RPC\n");
    print_error(ret);
  }
  for (;;);
  return (0);
}
