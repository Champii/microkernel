/************************************************
*
* Micro Kernel
*
* - pic functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __PIC_H__
# define                  __PIC_H__

# define                  PIC_MASTER_COM  0x20
# define                  PIC_MASTER_DATA 0x21
# define                  PIC_SLAVE_COM   0xA0
# define                  PIC_SLAVE_DATA  0xA1

void                      irq_set_mask(unsigned char irq);
void                      irq_clear_mask(unsigned char irq);
void                      init_pic();

#endif                    /*__PIC_H__*/
