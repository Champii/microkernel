/*
 * File: mman.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: memory mapping functions
 *
 */

#include <rpc/paging.h>
#include <sys/mman.h>

void* mmap(void *vaddr, int rights, size_t size)
{
  return rpc_mmap(vaddr, rights, size);
}

void* mmap_phys(void *vaddr, void *paddr, int rights, size_t size)
{
  return rpc_mmap_phys(vaddr, paddr, rights, size);
}

int munmap(void *vaddr)
{
  return rpc_munmap(vaddr);
}
