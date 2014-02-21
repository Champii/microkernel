/************************************************
*
* Micro Kernel
*
* - Services funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __SERVICES_H__
# define                  __SERVICES_H__

#include                  "multiboot.h"

void                      init_services(unsigned count, struct s_multiboot_module *module);

#endif                    /*__SERVICES_H__*/
