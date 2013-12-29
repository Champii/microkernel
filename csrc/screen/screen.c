/************************************************
*
* Micro Kernel
*
* - Standard printing funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include      "screen.h"

int           screen_x = 0;
int           screen_y = 0;
int           color_byte;
unsigned short *screen = (unsigned short *)SCREEN_PTR

void          change_color(int color)
{
  color_byte = (0 << 4) | (color & 0x0F);
}

void          printch(char c)
{
  int         width;
  unsigned short color;
  unsigned short *offset;

  width = SCREEN_WIDTH;

  color = color_byte << 8;

  offset = screen + (screen_y * width + screen_x);
  *offset = c | color;

  screen_x++;
  if (screen_x >= width)
  {
    screen_x = 0;
    screen_y++;
  }
}

void          printk(int color, char *str)
{
  change_color(color);

  while (*str)
  {
    printch(*str);
    str++;
  }
}
