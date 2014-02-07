/************************************************
*
* Micro Kernel
*
* - pit funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "pit.h"
#include                  "isrs.h"
#include                  "irq.h"
#include                  "screen.h"
#include                  "system.h"
#include                  "task.h"

unsigned                  tick = 0;

static void timer_callback(struct s_regs *regs)
{
   tick++;
   switch_task(regs);
}

void                      init_pit(unsigned frequency)
{
   register_interrupt_handler(IRQ0, &timer_callback);

   unsigned divisor = 1193180 / frequency;

   outportb(PIT_CR_PORT, 0x36);

   unsigned char l = (unsigned char)(divisor & 0xFF);
   unsigned char h = (unsigned)( (divisor>>8) & 0xFF );

   outportb(PIT_PORT_1, l);
   outportb(PIT_PORT_1, h);
}
