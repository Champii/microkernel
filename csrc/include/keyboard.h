/************************************************
*
* Micro Kernel
*
* - Keyboard funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __KEYBOARD_H__
# define              __KEYBOARD_H__

# define              KEY_OUTPUT_PORT 0x60
# define              KEY_CR_PORT     0x64

void                  init_keyboard();

#endif                /*__KEYBOARD_H__*/
