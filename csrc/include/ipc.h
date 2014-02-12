/************************************************
*
* Micro Kernel
*
* - Ipc header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __IPC_H__
# define              __IPC_H__

# include             "process.h"

void                  send(u64 pid, void *msg, unsigned size);
void                  recv(u64 pid, void *pool, unsigned pool_size);

#endif                /*__IPC_H__*/