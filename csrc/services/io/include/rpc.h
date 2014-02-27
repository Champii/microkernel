/************************************************
*
* Micro Kernel
*
* - Program Loader RPC header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __IO_RPC_H__
# define                  __IO_RPC_H__

# include                 <sys/sys_types.h>

// RPC Handlers
void                      write_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      write_at_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      read_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      read_one_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);

// Register handlers
void                      register_listen_rpcs();

#endif                    /*__IO_RPC_H__*/

