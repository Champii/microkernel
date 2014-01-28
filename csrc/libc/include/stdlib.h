/*
 * File: stdlib.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: Minimal stdlib.h
 *
 */

#ifndef STDLIB_H_
# define STDLIB_H_

# include <stddef.h>
# include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);
void* calloc(size_t nmenb, size_t size);

__attribute__((noreturn)) void exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !STDLIB_H_ */
