/************************************************
*
* Micro Kernel
*
* - Program Loader RPC funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <stdio.h>
#include                  <unistd.h>
#include                  <errno.h>

#include                  <rpc.h>

#define                   COLOR_WHITE 15

extern char               *rpc_pl_desc[4];

void                      register_listen_rpcs()
{
  struct rpc rpcs[4];
  rpcs[0].func_desc = rpc_pl_desc[0];
  rpcs[0].handler = &exec_rpc;
  rpcs[1].func_desc = rpc_pl_desc[1];
  rpcs[1].handler = &exit_rpc;
  rpcs[2].func_desc = rpc_pl_desc[2];
  rpcs[2].handler = &kill_rpc;
  rpcs[3].func_desc = rpc_pl_desc[3];
  rpcs[3].handler = &service_pid_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, 4)) < 0)
  {
    kwrite(COLOR_WHITE, "PL: Error Register RPC\n", 0);
    print_error(ret);
  }

  if ((ret = listen_rpc()) < 0)
  {
    kwrite(COLOR_WHITE, "PL: Error Listen rpc RPC\n", 0);
    print_error(ret);
  }

}

