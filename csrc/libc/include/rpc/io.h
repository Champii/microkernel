/*
 * File: rpc/io.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC IO functions for EPITECH K II
 *
 */

#ifndef RPC_IO_H_
# define RPC_IO_H_

#include <sys/types.h>

#define RPC_IO_WRITE    0
#define RPC_IO_WRITE_AT 1
#define RPC_IO_READ     2
#define RPC_IO_READ_ONE 3
#define RPC_IO_KERNEL   4

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

const char *rpc_io_desc[4];

int rpc_write(int color, const char *str, unsigned size);
int rpc_write_at(int x, int y, int color, char c);
int rpc_read(char *buff, u32 size);
char rpc_read_one(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !RPC_IO_H_ */
