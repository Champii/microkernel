/*
 * This file is license free, use it as you want!
 */

#include                  "keymap.h"

unsigned char             keymap_us[128] =
{
  0, KEY_ESCAPE,
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '-', '=',
  KEY_BACKSPACE, KEY_TAB,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
  KEY_ENTER, KEY_CTRL,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
  KEY_LSHIFT,
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
  KEY_RSHIFT,
  '*',
  KEY_ALT, KEY_SPACE, KEY_MAJLOCK,
  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
  KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,
  KEY_NUMLOCK, KEY_SCROLLLOCK, KEY_HOME, KEY_UP, KEY_PAGEUP,
  '-',
  KEY_LEFT, 0, KEY_RIGHT,
  '+',
  KEY_END, KEY_DOWN, KEY_PAGEDOWN, KEY_INSERT, KEY_DEL,
  0, 0, 0,
  KEY_F11, KEY_F12,
  0
};
