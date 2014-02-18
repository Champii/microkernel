/************************************************
*
* Micro Kernel
*
* - Get argument from params
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <string.h>
#include                  <sys/sys_types.h>

int                       get_int_arg(void **params)
{
  int                     ret = **((int **)params);

  *params += sizeof(int);

  return ret;
}

unsigned                  get_unsigned_arg(void **params)
{
  unsigned                ret = **((unsigned **)params);

  *params += sizeof(unsigned);

  return ret;
}

u64                       get_u64_arg(void **params)
{
  u64                ret = **((u64 **)params);

  *params += sizeof(u64);

  return ret;
}

unsigned                  get_str_arg(void **params, char *str)
{
  unsigned                str_size;

  str_size = get_unsigned_arg(params);

  memcpy(str, *params, str_size);

  *params += str_size;

  return str_size;
}
