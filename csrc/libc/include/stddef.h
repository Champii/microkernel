/*
 * File: stddef.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: Stddef as defined by POSIX
 *
 */

#ifndef STDDEF_H_
# define STDDEF_H_

# define NULL ((void*)0)

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#endif /* !STDDEF_H_ */
