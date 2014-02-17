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

void                      write_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  unsigned *uparams = (unsigned *)params;
  char str[1024];

  sender = sender;

  unsigned color = (unsigned)*uparams;
  uparams += 1;
  unsigned str_size = *((unsigned *)uparams);
  uparams += 1;
  memcpy(str, (char *)uparams, str_size);
  uparams = (unsigned *)(((char *)uparams) + str_size);

  printk(color, str);

  *ret = 0;
  *ret_size = 4;
}