#include <rpc/rpc.h>
#include <unistd.h>

#define COLOR_WHITE 15

void print_error(int ret)
{
  kwrite(COLOR_WHITE, "Error : ", 0);
  switch (ret)
  {
    case -1:
      kwrite(COLOR_WHITE, "No more memory\n", 0);
      break;
    case -2:
      kwrite(COLOR_WHITE, "Operation not permitted\n", 0);
      break;
    case -3:
      kwrite(COLOR_WHITE, "Invalid argument\n", 0);
      break;
    case -4:
      kwrite(COLOR_WHITE, "No such process\n", 0);
      break;
    case -5:
      kwrite(COLOR_WHITE, "Bad address\n", 0);
      break;
    case -6:
      kwrite(COLOR_WHITE, "Bad message\n", 0);
      break;
    case -7:
      kwrite(COLOR_WHITE, "Message too long\n", 0);
      break;

  }
}

void write_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  kwrite(COLOR_WHITE, "WRITE RPC !\n", 0);
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
}


int main()
{
  struct rpc rpcs[4];
  rpcs[0].func_desc = "iiii";
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
    kwrite(COLOR_WHITE, "Error Register RPC\n", 0);
    print_error(ret);

  }

  // sys_send(prog_loader_pid, "lol", 4);
  if ((ret = listen_rpc()) < 0)
  {
    kwrite(COLOR_WHITE, "Error Listen rpc RPC\n", 0);
    print_error(ret);
  }
  for (;;);
  return (0);
}
