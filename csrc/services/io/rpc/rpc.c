/************************************************
*
* Micro Kernel
*
* - IO RPC funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <stdio.h>
#include                  <unistd.h>
#include                  <errno.h>
#include                  <screen.h>

#include                  <rpc.h>
#include                  <restrict_rpc.h>

#define                   COLOR_WHITE 15

extern const char         *rpc_io_desc[4];

handler_rpc               rpcs[4];

void                      register_listen_rpcs()
{
  // struct rpc rpcs[4];
  init_restrict_rpc();

  rpcs[0] = (handler_rpc)&write_rpc;
  rpcs[1] = (handler_rpc)&write_at_rpc;
  rpcs[2] = (handler_rpc)&read_rpc;
  rpcs[3] = (handler_rpc)&read_one_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, rpc_io_desc, 4)) < 0)
  {
    kwrite(COLOR_WHITE, "IO: Error Register RPC\n", 0);
    print_error(ret);
  }

  printk(COLOR_WHITE, "IO service listening...\n");
  if ((ret = listen_rpc()) < 0)
  {
    kwrite(COLOR_WHITE, "IO: Error Listen rpc RPC\n", 0);
    print_error(ret);
  }

}

