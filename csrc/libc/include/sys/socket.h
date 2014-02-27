/*
 * File: sys/socket.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Unix socket functions adapted to EPITECH K II
 *
 */

#ifndef SOCKET_H_
# define SOCKET_H_

#include <sys/types.h>

#define RCV_ANYONE 0
#define MSG_MAXBUFF 0x1000
#define MSG_MAXSIZE (MSG_MAXBUFF - 8)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int send(u64 to, void *msg, u32 size);
int recv(u64 from, void *pool, u32 pool_size, u64 *pid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !SOCKET_H_ */
