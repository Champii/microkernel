/************************************************
*
* Micro Kernel
*
* - IO Write RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <string.h>
#include                  <screen.h>
#include                  <rpc.h>
#include                  <get_argument.h>

void                      write_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  char str[1024];

  sender = sender;
  param_size = param_size;

  unsigned color = get_unsigned_arg(&params);
  unsigned str_size = get_str_arg(&params, str);

  str_size = str_size;

  printk(color, str);

  *(unsigned *)ret = 0;
  *ret_size = 4;
}