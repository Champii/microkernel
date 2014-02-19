/************************************************
*
* Micro Kernel
*
* - Restrict RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __RESTRICT_RPC_H__
# define                  __RESTRICT_RPC_H__

void                      init_restrict_rpc();
int                       check_restrict_rpc(unsigned func_id, u64 sender);
void                      restrict_rpc(unsigned func_id, u64 sender);


#endif                    /*__RESTRICT_RPC_H__*/
