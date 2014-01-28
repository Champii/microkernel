/*
 * File: rpc/rpc.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC for EPITECH K II
 *
 */

#ifndef RPC_H_
# define RPC_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* void handler_rpc(u64 sender, void *params, void **ret, u32 *ret_size); */
typedef void (*handler_rpc) (u64, void *, void **, u32 *);

struct rpc
{
  const char *func_desc;
  handler_rpc handler;
};

int call_rpc(u64 pid, u32 funcion_id, const char *func_desc, void *ret,
    u32 ret_size, ...);
int register_rpc(struct rpc *rpcs, unsigned size);
int listen_rpc(void);

extern u64 prog_loader_pid;
int paging_service_pid(u64 *pid);
int io_service_pid(u64 *pid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !RPC_H_ */
