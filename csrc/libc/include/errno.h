/*
 * File: errno.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: Unix errors adapted to EPITECH K II
 *
 */

#ifndef ERRNO_H_
# define ERRNO_H_

#define ENOMEM    1  /* Memory error */
#define EPERM     2  /* Operation not permitted */
#define EINVAL    3  /* Invalid argument */
#define ESRCH     4  /* No such process */
#define EFAULT    5  /* Bad address */
#define EBADMSG   6  /* Bad message */
#define EMSGSIZE  7  /* Message too long */

void print_error(int ret);


#endif /* !ERRNO_H_ */
