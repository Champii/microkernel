/*
 * File: sleep.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: sleep function
 *
 */

#include <sys/syscall.h>
#include <unistd.h>

int sleep(u32 milli)
{
  return sys_sleep(milli);
}
