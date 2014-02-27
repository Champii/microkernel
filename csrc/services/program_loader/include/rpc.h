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

struct                    s_program
{
  char                    name[32];
  unsigned                *elf_addr;
};


// RPC Handlers
void                      exec_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      exit_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      kill_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      service_pid_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);

// Register handlers
void                      register_listen_rpcs();

#endif                    /*__PL_RPC_H__*/

