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
#include                  <get_argument.h>

extern u64                paging_pid;
extern u64                io_pid;

void                      service_pid_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  char                    tmp[1024];

  sender = sender;
  param_size = param_size;

  get_str_arg(&params, tmp);

  u64 *res;

  if (!strncmp(tmp, "paging", 6))
    res = &paging_pid;
  else if (!strncmp(tmp, "io", 2))
    res = &io_pid;
  else
    res = 0;

  memcpy(ret, res, sizeof(u64));
  // *(unsigned *)ret = res;
  *ret_size = 8;
}
