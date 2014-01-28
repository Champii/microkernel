/*
 * File: wait.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: wait function
 *
 */

#include <sys/syscall.h>
#include <sys/wait.h>

int wait(u64 pid)
{
  return sys_wait(pid);
}
