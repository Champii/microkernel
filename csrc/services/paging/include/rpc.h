/************************************************
*
* Micro Kernel
*
* - Program Loader RPC header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __PAGING_RPC_H__
# define                  __PAGING_RPC_H__

# include                 <sys/sys_types.h>

// RPC Handlers
void                      mmap_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      mmap_phys_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      munmap_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);

// Private RPC to process loader
void                      _create_as_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      _delete_as_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);
void                      _mmap_sys_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size);

// Register handlers
void                      register_listen_rpcs();

#endif                    /*__PAGING_RPC_H__*/

