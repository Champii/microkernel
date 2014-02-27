/*
 * File: signal.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Unix signal function adapted to EPITECH K II
 *
 */

#ifndef SIGNAL_H_
# define SIGNAL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int kill(u64 pid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !SIGNAL_H_ */
