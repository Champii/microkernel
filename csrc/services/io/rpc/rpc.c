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
#include                  <screen.h>

#include                  <rpc.h>

#define                   COLOR_WHITE 15

extern char               *rpc_io_desc[4];

void                      register_listen_rpcs()
{
  struct rpc rpcs[4];
  rpcs[0].func_desc = rpc_io_desc[0];
  rpcs[0].handler = &write_rpc;
  rpcs[1].func_desc = rpc_io_desc[1];
  rpcs[1].handler = &write_at_rpc;
  rpcs[2].func_desc = rpc_io_desc[2];
  rpcs[2].handler = &read_rpc;
  rpcs[3].func_desc = rpc_io_desc[3];
  rpcs[3].handler = &read_one_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, 4)) < 0)
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

