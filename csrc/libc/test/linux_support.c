/*
 * File: linux_support.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Linux support for testing purpose
 *
 */

#include "linux_support.h"

extern void _start(u64 pid);

void real_start()
{
  _start(1);
}
