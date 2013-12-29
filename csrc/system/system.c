/************************************************
*
* Micro Kernel
*
* - Standard system funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

extern unsigned char  *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
  int                 i;

  for (i = 0; i < count; i++)
    dest[i] = src[i];

  return dest;
}

extern unsigned char  *memset(unsigned char *dest, unsigned char val, int count)
{
  int                 i;

  for (i = 0; i < count; i++)
    dest[i] = val;

  return dest;
}

extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
  int                 i;

  for (i = 0; i < count; i++)
    dest[i] = val;

  return dest;
}

extern int            strlen(const char *str)
{
  int                 len = 0;

  while (*str)
  {
    len++;
    str++;
  }

  return len;
}

extern unsigned char  inportb(unsigned short _port)
{
  unsigned char rv;

  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));

  return rv;
}

extern void           outportb(unsigned short _port, unsigned char _data)
{
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
