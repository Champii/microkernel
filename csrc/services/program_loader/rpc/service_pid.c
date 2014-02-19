/************************************************
*
* Micro Kernel
*
* - Program Loader Service_pid RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <string.h>
#include                  <rpc.h>

extern u64                *paging_pid;
extern u64                *io_pid;

void                      service_pid_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  char                    *tmp;

  sender = sender;

  tmp = (char *)(params + sizeof(u32));

  u64 *res;
  if (!strncmp(tmp, "paging", 6))
    res = paging_pid;
  else if (!strncmp(tmp, "io", 2))
    res = io_pid;
  else
    res = 0;

  *ret = res;
  *ret_size = 8;
}
