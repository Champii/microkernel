/************************************************
*
* Micro Kernel
*
* - Standard printing funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include          "screen.h"
#include          "system.h"

int               screen_x = 0;
int               screen_y = 0;
int               color_byte;
unsigned short    *screen = (unsigned short *)SCREEN_PTR

void              move_cursor()
{
  unsigned        offset;
  int             width;

  width = SCREEN_WIDTH;
  offset = screen_y * width + screen_x;
  outportb(0x3D4, 14);
  outportb(0x3D5, offset >> 8);
  outportb(0x3D4, 15);
  outportb(0x3D5, offset);
}

void              change_color(int color)
{
  color_byte = (0 << 4) | (color & 0x0F);
}

void              scroll()
{
  int             height = SCREEN_HEIGHT;
  int             offset;

  if (screen_y >= height)
  {
    offset = screen_y - height + 1;
    memcpy(screen, screen + offset * 80, (height - offset) * 80 * 2);

    memsetw(screen + (height - offset) * 80, 15, 80);
    screen_y = height - 1;
  }
}

void              printch(char c)
{
  int             width;
  unsigned short  color;
  unsigned short  *offset;

  width = SCREEN_WIDTH;

  color = color_byte << 8;

  if (c == '\n')
  {
    screen_x = 0;
    screen_y++;
  }
  else
  {
    offset = screen + (screen_y * width + screen_x);
    *offset = c | color;
    screen_x++;
  }

  if (screen_x >= width)
  {
    screen_x = 0;
    screen_y++;
  }

  scroll();
  move_cursor();
}

void            printk(int color, char *str)
{
  change_color(color);

  while (*str)
  {
    printch(*str);
    str++;
  }
}
