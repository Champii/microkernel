/************************************************
*
* Micro Kernel
*
* - Program Loader Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/


#include <rpc/rpc.h>
#include <unistd.h>

#define COLOR_WHITE 15

void print_error(int ret)
{
  write(COLOR_WHITE, "Error : ", 0);
  switch (ret)
  {
    case -1:
      write(COLOR_WHITE, "No more memory\n", 0);
      break;
    case -2:
      write(COLOR_WHITE, "Operation not permitted\n", 0);
      break;
    case -3:
      write(COLOR_WHITE, "Invalid argument\n", 0);
      break;
    case -4:
      write(COLOR_WHITE, "No such process\n", 0);
      break;
    case -5:
      write(COLOR_WHITE, "Bad address\n", 0);
      break;
    case -6:
      write(COLOR_WHITE, "Bad message\n", 0);
      break;
    case -7:
      write(COLOR_WHITE, "Message too long\n", 0);
      break;

  }
}


void service_pid_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
  write(COLOR_WHITE, "SENDER PID RPC !!!!!!!!!!!!!", 0);
  // sys_send(sender, params, 4);
}

int main()
{

  write(COLOR_WHITE, "Starting Program Loader service\n", 0);

  struct rpc rpcs[4];
  rpcs[0].func_desc = "Ii";
  rpcs[0].handler = &service_pid_rpc;
  rpcs[1].func_desc = "Ii";
  rpcs[1].handler = &service_pid_rpc;
  rpcs[2].func_desc = "Ii";
  rpcs[2].handler = &service_pid_rpc;
  rpcs[3].func_desc = "Ii";
  rpcs[3].handler = &service_pid_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, 4)) < 0)
  {
    write(COLOR_WHITE, "Error Register RPC\n", 0);
    print_error(ret);

  }

  // sys_send(prog_loader_pid, "lol", 4);
  if ((ret = listen_rpc()) < 0)
  {
    write(COLOR_WHITE, "Error Listen rpc RPC\n", 0);
    print_error(ret);
  }

  for (;;);

  return (0);
}
