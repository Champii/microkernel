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

extern u64                prog_loader_pid;

extern const char         *rpc_paging_desc[6];

handler_rpc               rpcs[6];

void                      register_listen_rpcs()
{
  // struct rpc              rpcs[6];

  init_restrict_rpc();


  rpcs[0] = (handler_rpc)&mmap_rpc;
  rpcs[1] = (handler_rpc)&mmap_phys_rpc;
  rpcs[2] = (handler_rpc)&munmap_rpc;
  rpcs[3] = (handler_rpc)&_create_as_rpc;
  rpcs[4] = (handler_rpc)&_delete_as_rpc;
  rpcs[5] = (handler_rpc)&_mmap_sys_rpc;

  // restrict_rpc(3, prog_loader_pid);
  // restrict_rpc(4, prog_loader_pid);
  // restrict_rpc(5, prog_loader_pid);

  int ret;
  if ((ret = register_rpc(rpcs, rpc_paging_desc, 6)) < 0)
  {
    kwrite(COLOR_WHITE, "PAGING: Error Register RPC\n", 0);
    print_error(ret);
  }


  // write(COLOR_WHITE, "Paging service listening...\n", 0);
  if ((ret = listen_rpc()) < 0)
  {
    kwrite(COLOR_WHITE, "PAGING: Error Listen rpc RPC\n", 0);
    print_error(ret);
  }

}

