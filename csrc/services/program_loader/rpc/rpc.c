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
#include                  <restrict_rpc.h>

#define                   COLOR_WHITE 15

extern const char         *rpc_pl_desc[4];

handler_rpc               rpcs[4];

void                      register_listen_rpcs()
{
  // struct rpc              rpcs[4];

  init_restrict_rpc();

  rpcs[0] = (handler_rpc)&exec_rpc;
  rpcs[1] = (handler_rpc)&exit_rpc;
  rpcs[2] = (handler_rpc)&kill_rpc;
  rpcs[3] = (handler_rpc)&service_pid_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, rpc_pl_desc, 4)) < 0)
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

