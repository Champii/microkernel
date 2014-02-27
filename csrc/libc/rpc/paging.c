/*
 * File: paging.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: RPC for paging service
 *
 */

#include <rpc/paging.h>
#include <rpc/rpc.h>
#include <stdlib.h>

const char *rpc_paging_desc[6] =
{
  "iiii",   // mmap
  "iiiii",  // mmap_sys
  "ii",     // munmap
  "iI",     // create_as
  "iI",     // delete as
  "iIiii"   // mmap_sys
};

void *rpc_mmap(void *vaddr, int rights, unsigned size)
{
  u64 paging_pid;
  void *res;

  if (paging_service_pid(&paging_pid) < 0)
    return NULL;

  int sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_MMAP,
      rpc_paging_desc[RPC_PAGING_MMAP],
      (void *) &res,
      0,
      vaddr,
      rights,
      size);

  if (sys_ret < 0)
    return NULL;

  return res;
}

void *rpc_mmap_phys(void *vaddr, void *paddr, int rights, unsigned size)
{
  u64 paging_pid;
  void *res;

  if (paging_service_pid(&paging_pid) < 0)
    return NULL;

  int sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_MMAP_PHYS,
      rpc_paging_desc[RPC_PAGING_MMAP_PHYS],
      (void *) &res,
      0,
      vaddr,
      paddr,
      rights,
      size);

  if (sys_ret < 0)
    return NULL;

  return res;
}

int rpc_munmap(void *vaddr)
{
  u64 paging_pid;
  int res;
  int sys_ret;

  sys_ret = paging_service_pid(&paging_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_MUNMAP,
      rpc_paging_desc[RPC_PAGING_MUNMAP],
      (void *) &res,
      0,
      vaddr);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_create_as(u64 pid)
{
  u64 paging_pid;
  int res;
  int sys_ret;

  sys_ret = paging_service_pid(&paging_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_CREATE_AS,
      rpc_paging_desc[RPC_PAGING_CREATE_AS],
      (void *) &res,
      0,
      pid);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_del_as(u64 pid)
{
  u64 paging_pid;
  int res;
  int sys_ret;

  sys_ret = paging_service_pid(&paging_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_DEL_AS,
      rpc_paging_desc[RPC_PAGING_DEL_AS],
      (void *) &res,
      0,
      pid);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

void *rpc_mmap_sys(u64 pid, void *vaddr, int rights, unsigned size)
{
  u64 paging_pid;
  void *res;

  if (paging_service_pid(&paging_pid) < 0)
    return NULL;

  int sys_ret = call_rpc(
      paging_pid,
      RPC_PAGING_MMAP_SYS,
      rpc_paging_desc[RPC_PAGING_MMAP_SYS],
      (void *) &res,
      0,
      pid,
      vaddr,
      rights,
      size);

  if (sys_ret < 0)
    return NULL;

  return res;
}
