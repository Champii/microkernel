/************************************************
*
* Micro Kernel
*
* - Program Loader Kill RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc.h>

void                      munmap_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
  param_size = param_size;
}
