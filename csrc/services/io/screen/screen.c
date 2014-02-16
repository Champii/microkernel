/************************************************
*
* Micro Kernel
*
* - Standard printing funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include          <string.h>
#include          "screen.h"
// #include          "system.h"
// #include          "kmalloc.h"
// #include          "mm.h"

int               screen_x = 0;
int               screen_y = 0;
int               color_byte;
unsigned short    *screen = (unsigned short *)SCREEN_PTR;


// char    *my_putnbr_base(unsigned nbr, char *base)
// {
//   int   diviseur;
//   int   basenb;
//   char    *res;
//   int   i;

//   i = 0;
//   basenb = strlen(base);
//   diviseur = 1;
//   res = kmalloc(10 * sizeof(*res));
//   if (nbr < 0)
//   {
//     res[i++] = '-';
//     nbr = -nbr;
//   }
//   while ((nbr / diviseur) >= basenb)
//   {
//     diviseur = diviseur * basenb;
//   }
//   while (diviseur >= 1)
//   {
//     res[i++] = base[(nbr / diviseur) % basenb];
//     nbr = nbr % diviseur;
//     diviseur = diviseur / basenb;
//   }
//   return (res);
// }

void              move_cursor()
{
  unsigned        offset = screen_y * SCREEN_WIDTH + screen_x;

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
  int             offset;

  if (screen_y >= SCREEN_HEIGHT)
  {
    offset = screen_y - SCREEN_HEIGHT + 1;
    memcpy(screen, screen + offset * 80, (SCREEN_HEIGHT - offset) * 80 * 2);

    memsetw(screen + (SCREEN_HEIGHT - offset) * 80, 15, 80);
    screen_y = SCREEN_HEIGHT - 1;
  }
}

void              clear_screen()
{
  unsigned short  color;

  change_color(COLOR_WHITE);
  color = color_byte << 8;

  memsetw(screen, ' ' | color, SCREEN_WIDTH * SCREEN_HEIGHT * 2);

  screen_x = 0;
  screen_y = 0;

  move_cursor();
}

void              printch(char c)
{
  unsigned short  color = color_byte << 8;
  unsigned short  *offset;

  if (c == '\n')
  {
    screen_x = 0;
    screen_y++;
  }
  else
  {
    offset = screen + (screen_y * SCREEN_WIDTH + screen_x);
    *offset = c | color;
    screen_x++;
  }

  if (screen_x >= SCREEN_WIDTH)
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

int bool(int n)
{
  int flag;

  if (n < -2147483647)
    flag = 1;
  else
    flag = 0;
  return (flag);
}

// int my_put_nbr(unsigned nb)
// {
//   int div;
//   int flag;

//   change_color(COLOR_WHITE);

//   div = 1;
//   flag = 0;
//   if (nb < 0)
//   {
//     flag = bool(nb);
//     if (flag == 1)
//       nb = -2147483647;
//     printch('-');
//     nb = 0 - nb;
//   }
//   while ((nb / div) >= 10)
//     div = div * 10;
//   while (div)
//   {
//     if (flag == 1 && div == 1)
//       printch((nb / div) % 10 + 49);
//     if (flag == 0 || (flag == 1 && div != 1))
//       printch((nb / div) % 10 + 48);
//     div = div / 10;
//   }
//   return (0);
// }
