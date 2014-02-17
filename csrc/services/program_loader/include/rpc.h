/************************************************
*
* Micro Kernel
*
* - Program Loader RPC header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __PL_RPC_H__
# define                  __PL_RPC_H__

# include                 <sys/sys_types.h>

// RPC Handlers
void                      exec_rpc(u64 sender, void *params, void **ret, unsigned *ret_size);
void                      exit_rpc(u64 sender, void *params, void **ret, unsigned *ret_size);
void                      kill_rpc(u64 sender, void *params, void **ret, unsigned *ret_size);
void                      service_pid_rpc(u64 sender, void *params, void **ret, unsigned *ret_size);

// Register handlers
void                      register_listen_rpcs();

#endif                    /*__PL_RPC_H__*/

