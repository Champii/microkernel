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

extern const char         *rpc_paging_desc[6];

void                      register_listen_rpcs()
{
  struct rpc              rpcs[6];

  rpcs[0].func_desc = rpc_paging_desc[0];
  rpcs[0].handler = &mmap_rpc;
  rpcs[1].func_desc = rpc_paging_desc[1];
  rpcs[1].handler = &mmap_sys_rpc;
  rpcs[2].func_desc = rpc_paging_desc[2];
  rpcs[2].handler = &munmap_rpc;
  rpcs[3].func_desc = rpc_paging_desc[3];
  rpcs[3].handler = &_create_as_rpc;
  rpcs[3].func_desc = rpc_paging_desc[4];
  rpcs[3].handler = &_delete_as_rpc;
  rpcs[3].func_desc = rpc_paging_desc[5];
  rpcs[3].handler = &_mmap_sys_rpc;

  int ret;
  if ((ret = register_rpc(rpcs, 4)) < 0)
  {
    kwrite(COLOR_WHITE, "PAGING: Error Register RPC\n", 0);
    print_error(ret);
  }


  write(COLOR_WHITE, "Paging service listening...\n", 0);
  if ((ret = listen_rpc()) < 0)
  {
    kwrite(COLOR_WHITE, "PAGING: Error Listen rpc RPC\n", 0);
    print_error(ret);
  }

}

