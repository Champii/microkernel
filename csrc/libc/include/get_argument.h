/************************************************
*
* Micro Kernel
*
* - Get argument from params
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __GET_ARGUMENT_H__
# define                  __GET_ARGUMENT_H__

int                       get_int_arg(void **params);
unsigned                  get_unsigned_arg(void **params);
u64                       get_u64_arg(void **params);
unsigned                  get_str_arg(void **params, char *str);

#endif                    /*__GET_ARGUMENT_H__*/
