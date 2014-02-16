/*
 * File: unistd.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: Unix standard functions adapted to EPITECH K II
 *
 */

#ifndef UNISTD_H_
# define UNISTD_H_

#define BW_COLOR 15

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int exec(char *name);

int kwrite(int color, const char *str, unsigned size);
int write(int color, const char *str, unsigned size);
int write_at(int x, int y, int color, char c);

int read(char *buff, unsigned size);
char read_one();

int sleep(u32 milli);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !UNISTD_H_ */
