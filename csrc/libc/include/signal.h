/*
 * File: signal.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
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
