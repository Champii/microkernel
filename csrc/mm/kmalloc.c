/************************************************
*
* Micro Kernel
*
* - Standard KMalloc functs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/
#include                    "mm.h"
#include                    "kmalloc.h"

extern unsigned           end;
unsigned                  placement_address = (unsigned)&end;

void                      *_kmalloc(unsigned size, int align, unsigned *phys)
{
  void                    *tmp;

  if (align && placement_address & 0xFFFFF000)
  {
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
    *phys = placement_address - 0xC0000000;

  tmp = (void *)placement_address;
  placement_address += size;
  return (tmp);
}

void                      *kmalloc(unsigned size)
{
  return _kmalloc(size, 0, 0);
}

void                      *kmalloc_a(unsigned size)
{
  return _kmalloc(size, 1, 0);
}

void                      *kmalloc_p(unsigned size, unsigned *phys)
{
  return _kmalloc(size, 0, phys);
}

void                      *kmalloc_ap(unsigned size, unsigned *phys)
{
  return _kmalloc(size, 1, phys);
}
