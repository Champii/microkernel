/*
 * File: kill.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: kill function
 *
 */

#include <rpc/program_loader.h>
#include <signal.h>

int kill(u64 pid)
{
  return rpc_kill(pid);
}
