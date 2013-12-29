/************************************************
*
* Micro Kernel
*
* - Standard system functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __SYSTEM_H__
# define              __SYSTEM_H__

extern void           *memcpy(void *dest, const void *src, int count);
extern void           *memset(void *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int            strlen(const char *str);
extern unsigned char  inportb(unsigned short _port);
extern void           outportb(unsigned short _port, unsigned char _data);

#endif                /*__SYSTEM_H__*/