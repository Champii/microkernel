/*
 * File: rpc/paging.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: RPC paging functions for EPITECH K II
 *
 */

#ifndef RPC_PAGING_H_
# define RPC_PAGING_H_

#include <sys/types.h>

#define RPC_PAGING_MMAP       0
#define RPC_PAGING_MMAP_PHYS  1
#define RPC_PAGING_MUNMAP     2
#define RPC_PAGING_CREATE_AS  3
#define RPC_PAGING_DEL_AS     4
#define RPC_PAGING_MMAP_SYS   5

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

const char *rpc_paging_desc[6];

void *rpc_mmap(void *vaddr, int rights, unsigned size);
void *rpc_mmap_phys(void *vaddr, void *paddr, int rights, unsigned size);
int rpc_munmap(void *vaddr);

int rpc_create_as(u64 pid);
int rpc_del_as(u64 pid);
void *rpc_mmap_sys(u64 pid, void *vaddr, int rights, unsigned size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !RPC_PAGING_H_ */
