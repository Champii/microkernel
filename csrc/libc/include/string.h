/*
 * File: string.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: libc string.h
 *
 */

#ifndef STRING_H_
# define STRING_H_

# include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

size_t strlen(const char* s);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t len);
char* strncpy(char* s1, const char* s2, size_t n);
void* memset(void* s1, int c, size_t n);
void* memcpy(void* s1, const void* s2, size_t n);
unsigned char  inportb(unsigned short _port);
void           outportb(unsigned short _port, unsigned char _data);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !STRING_H_ */
