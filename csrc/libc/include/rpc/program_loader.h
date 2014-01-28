/*
 * File: rpc/program_loader.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC paging functions for EPITECH K II
 *
 */

#ifndef RPC_PROGRAM_LOADER_H_
# define RPC_PROGRAM_LOADER_H_

#include <sys/types.h>

#define RPC_PL_EXEC         0
#define RPC_PL_EXIT         1
#define RPC_PL_KILL         2
#define RPC_PL_SERVICE_PID  3

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

const char *rpc_pl_desc[4];

int rpc_exec(char *name);
int rpc_exit();
int rpc_kill(u64 pid);
int rpc_service_pid(char *service_name, u64 *pid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !RPC_PROGRAM_LOADER_H_ */
