/*
 * This file is license free, use it as you want!
 *
 * 0 means no key code is corresponding to this value
 * KEY_xxxx is a special key
 */

#ifndef                   KEYMAP_H_
# define                  KEYMAP_H_

# define                  KEY_BACKSPACE   8
# define                  KEY_TAB         9
# define                  KEY_ENTER       '\n'
# define                  KEY_ESCAPE      27
# define                  KEY_SPACE       ' '

# define                  KEY_CTRL        128
# define                  KEY_LSHIFT      129
# define                  KEY_RSHIFT      130
# define                  KEY_ALT         131
# define                  KEY_MAJLOCK     132
# define                  KEY_NUMLOCK     133
# define                  KEY_SCROLLLOCK  134
# define                  KEY_HOME        135
# define                  KEY_END         136
# define                  KEY_INSERT      137
# define                  KEY_DEL         138

# define                  KEY_F1          140
# define                  KEY_F2          142
# define                  KEY_F3          143
# define                  KEY_F4          144
# define                  KEY_F5          145
# define                  KEY_F6          146
# define                  KEY_F7          147
# define                  KEY_F8          148
# define                  KEY_F9          149
# define                  KEY_F10         150
# define                  KEY_F11         151
# define                  KEY_F12         152

# define                  KEY_PAGEUP      160
# define                  KEY_PAGEDOWN    161

# define                  KEY_UP          162
# define                  KEY_DOWN        163
# define                  KEY_LEFT        164
# define                  KEY_RIGHT       165


unsigned char             keymap_us[128];

#endif                    /* !KEYMAP_H_ */