/*
 * File: exec.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: exec function
 *
 */

#include <rpc/program_loader.h>
#include <unistd.h>

int exec(char *name)
{
  return rpc_exec(name);
}
