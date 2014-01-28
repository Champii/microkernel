/*
 * File: program_loader.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC for program loader service
 *
 */

#include <rpc/program_loader.h>
#include <rpc/rpc.h>

const char *rpc_pl_desc[4] =
{
  "ic", // exec
  "i", // exit
  "iI", // kill
  "Ii" // service_pid
};

int rpc_exec(char *name)
{
  int res;

  int sys_ret = call_rpc(
      prog_loader_pid,
      RPC_PL_EXEC,
      rpc_pl_desc[RPC_PL_EXEC],
      (void *) &res,
      0,
      name);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_exit()
{
  int res;

  int sys_ret = call_rpc(
      prog_loader_pid,
      RPC_PL_EXIT,
      rpc_pl_desc[RPC_PL_EXIT],
      (void *) &res,
      0);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_kill(u64 pid)
{
  int res;

  int sys_ret = call_rpc(
      prog_loader_pid,
      RPC_PL_KILL,
      rpc_pl_desc[RPC_PL_KILL],
      (void *) &res,
      0,
      pid);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_service_pid(char *service_name, u64 *pid)
{
  int sys_ret = call_rpc(
      prog_loader_pid,
      RPC_PL_SERVICE_PID,
      rpc_pl_desc[RPC_PL_SERVICE_PID],
      (void *) pid,
      0,
      service_name);

  if (sys_ret < 0)
    return sys_ret;

  return 0;
}
