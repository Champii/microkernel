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

# define              IPC_PAGE_1_ADDR 0xC0400000
# define              IPC_TABLE_1_IDX (IPC_PAGE_1_ADDR/0x1000)/1024
# define              IPC_PAGE_1_IDX (IPC_PAGE_1_ADDR/0x1000)%1024

# define              IPC_PAGE_2_ADDR 0xC0401000
# define              IPC_TABLE_2_IDX (IPC_PAGE_2_ADDR/0x1000)/1024
# define              IPC_PAGE_2_IDX (IPC_PAGE_2_ADDR/0x1000)%1024

# include             "process.h"


int                   send(u64 pid, void *msg, unsigned size);
int                   recv(u64 pid, void *pool, unsigned pool_size);

#endif                /*__IPC_H__*/