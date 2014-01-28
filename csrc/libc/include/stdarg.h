/*
 * File: stdarg.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: va_args
 *
 */

#ifndef STDARG_H_
# define STDARG_H_

# define va_start(v,l) __builtin_va_start(v,l)
# define va_end(v) __builtin_va_end(v)
# define va_arg(v,l) __builtin_va_arg(v,l)
# define va_copy(d,s) __builtin_va_copy(d,s)

typedef __builtin_va_list va_list;

#endif /* !STDARG_H_ */
