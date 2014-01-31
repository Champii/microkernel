/************************************************
*
* Micro Kernel
*
* - pit functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __PIT_H__
# define                  __PIT_H__

# define                  PIT_CR_PORT 0x43
# define                  PIT_PORT_1  0x40

# define                  INTERNAL_FREQUENCY 1193182
# define                  DESIRED_FREQUENCY 100
# define                  DIVIDER INTERNAL_FREQUENCY/DESIRED_FREQUENCY

void                      init_pit();

#endif                    /*__PIT_H__*/
