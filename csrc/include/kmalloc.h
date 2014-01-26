/************************************************
*
* Micro Kernel
*
* - Standard KMalloc functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __KMALLOC_H__
# define              __KMALLOC_H__

/* Normal malloc */
void                  *kmalloc(unsigned size);

/* Aligned malloc */
void                  *kmalloc_a(unsigned size);

/* Set real address in *phys */
void                  *kmalloc_p(unsigned size, unsigned *phys);

/* Set real address in *phys and aligned */
void                  *kmalloc_ap(unsigned size, unsigned *phys);

#endif                /*__KMALLOC_H__*/
