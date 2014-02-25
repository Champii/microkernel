/*
 * File: paging.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC for paging service
 *
 */

#include <rpc/paging.h>
#include <rpc/rpc.h>
#include <stdlib.h>
#include <unistd.h>

const char *rpc_paging_desc[6] =
{
  "iiii",   // mmap
  "iiiii",  // mmap_phys
  "ii",     // munmap
  "iI",     // create_as
  "iI",     // delete as
  "iIiii"   // mmap_sys
};

unsigned itoa_base(int n, char *s, int base);
unsigned uitoa_base(unsigned n, char *s, int base);

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

  kwrite(15, "Start create as rpc\n", 0);
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

  char tmp[10];
  unsigned *pid_split = (unsigned *)&paging_pid;
  itoa_base(pid_split[0], tmp, 10);

  kwrite(15, "Hello rpc_create_as pag pid = ", 0);
  kwrite(15, tmp, 0);
  kwrite(15, "\n", 0);

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


  char tmp[10];
  itoa_base((int)res, tmp, 10);
  kwrite(15, "RPC MMAP SYS ret = ", 0);
  kwrite(15, tmp, 0);
  kwrite(15, "\n", 0);

  if (sys_ret < 0)
    return NULL;

  return res;
}
