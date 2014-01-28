/*
 * File: sleep.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
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
