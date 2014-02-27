/*
 * File: wait.c
 * Author: Victor Aperce <vaperce@gmail.com>
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
