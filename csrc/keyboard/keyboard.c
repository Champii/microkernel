/************************************************
*
* Micro Kernel
*
* - Keyboard funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include              "keyboard.h"
#include              "isrs.h"
#include              "irq.h"
#include              "system.h"
#include              "keymap.h"
#include              "screen.h"

static void           keyboard_callback(struct s_regs regs)
{
  unsigned            keycode;
  char                c[1];

  keycode = inportb(KEY_OUTPUT_PORT);
  if (keycode < 128)
  {
    c[0] = keymap_us[keycode];
    printk(COLOR_WHITE, c);
  }
}

void                  init_keyboard()
{
  register_interrupt_handler(IRQ1, &keyboard_callback);
}
