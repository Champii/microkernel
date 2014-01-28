/*
 * File: sys/wait.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: Unix wait function adapted for EPITECH K II
 *
 */

#ifndef WAIT_H_
# define WAIT_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int wait(u64 pid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !WAIT_H_ */
