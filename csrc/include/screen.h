/************************************************
*
* Micro Kernel
*
* - Standard printing functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __SCREEN_H__
# define              __SCREEN_H__

# define              SCREEN_PTR            0xB8000

# define              SCREEN_WIDTH          80
# define              SCREEN_HEIGHT         25

# define              COLOR_BLACK           0
# define              COLOR_BLUE            1
# define              COLOR_GREEN           2
# define              COLOR_CYAN            3
# define              COLOR_RED             4
# define              COLOR_MAGENTA         5
# define              COLOR_BROWN           6
# define              COLOR_LIGHT_GREY      7
# define              COLOR_DARK_GREY       8
# define              COLOR_LIGHT_BLUE      9
# define              COLOR_LIGHT_GREEN     10
# define              COLOR_LIGHT_CYAN      11
# define              COLOR_LIGHT_RED       12
# define              COLOR_LIGHT_MAGENTA   13
# define              COLOR_LIGHT_BROWN     14
# define              COLOR_WHITE           15

void                  printk(int color, char *str);
int                   my_put_nbr(unsigned nb);
char                  *my_putnbr_base(unsigned nbr, char *base);
void                  clear_screen();

#endif                /*__SCREEN_H__*/
