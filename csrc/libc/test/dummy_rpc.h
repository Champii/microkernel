/*
 * File: dummy_rpc.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Dummy RPC answers for testing purpose
 *
 */

#ifndef DUMMY_RPC_H_
#define DUMMY_RPC_H_

#include "sys/types.h"

#define IAM_NOTHING 0
#define IAM_SERV_PL 1
#define IAM_SERV_PG 2
#define IAM_SERV_IO 3

int dummy_rpc_init(int iam);
void dummy_rpc_clean(void);
int dummy_rpc_on_send(u64 to, void *msg, u32 size);
int dummy_rpc_on_recv(u64 from, void *msg, u32 size);

#endif /* !DUMMY_RPC_H_ */
