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
#include <string.h>

#define COLOR_WHITE 15

unsigned uitoa_base(unsigned n, char *s, int base);

extern unsigned *stack_start;

u64 *paging_pid = 0;
u64 *io_pid = 0;

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

unsigned itoa_base(int n, char *s, int base);

void service_pid_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  unsigned *test = (unsigned *)&sender;
  char str[10];
  char *tmp;

  // unsigned len = *(unsigned *)params;


  tmp = (char *)(params + 1);


  itoa_base(test[0], str, 10);

  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
  kwrite(COLOR_WHITE, "SENDER PID RPC  : ", 0);
  kwrite(COLOR_WHITE, str, 0);
  kwrite(COLOR_WHITE, "\n", 0);

  // itoa_base((unsigned)*(unsigned **)params, str, 16);
  kwrite(COLOR_WHITE, "PARAM 1 : ", 0);
  kwrite(COLOR_WHITE, (char *)tmp, 0);
  kwrite(COLOR_WHITE, "\n", 0);


  u64 *res;
  if (!strncmp(tmp, "paging", 7))
  {

    kwrite(COLOR_WHITE, "ASK PAGING PID !\n", 0);
    res = paging_pid;
  }
  else if (!strncmp(tmp, "io", 7))
  {

    kwrite(COLOR_WHITE, "ASK IO PID !\n", 0);
    res = io_pid;
  }

  *ret = res;
  *ret_size = 8;
  // sys_send(sender, params, 4);
}

void get_services_pid()
{
  io_pid = (u64 *)stack_start;
  paging_pid = (u64 *)(stack_start + 2);
}

int main()
{
  // unsigned *pid_split = (unsigned *)&prog_loader_pid;
  // char str[10];

  kwrite(COLOR_WHITE, "Starting Program Loader service\n", 0);

  get_services_pid();

  struct rpc rpcs[4];
  rpcs[0].func_desc = "Is";
  rpcs[0].handler = &service_pid_rpc;
  rpcs[1].func_desc = "Is";
  rpcs[1].handler = &service_pid_rpc;
  rpcs[2].func_desc = "Is";
  rpcs[2].handler = &service_pid_rpc;
  rpcs[3].func_desc = "Is";
  rpcs[3].handler = &service_pid_rpc;

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
