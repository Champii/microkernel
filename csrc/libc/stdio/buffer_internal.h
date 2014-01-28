/*
 * File: buffer_internal.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: simple buffer implemenation for printf
 *
 */

#ifndef BUFFER_INTERNAL_H_
# define BUFFER_INTERNAL_H_

# include <stddef.h>

struct buff
{
  char   *tab;
  size_t max;
  size_t size;
};

typedef struct buff s_buff;

/* Create a new buffer */
s_buff *make_buff(size_t max);

/* Add a string with size l to buffer and resize it if needed */
void add_string(s_buff *b, const char *str, size_t l);

/* Same as previous with a char */
void add_char(s_buff *b, char c);

/* Return the pointer to the string buffer */
char *get_string(s_buff *b);

/* Reset buffer */
void reset_buff(s_buff *b);

/* Free buffer */
void free_buff(s_buff *b);

/* Return the size used in the buffer */
size_t get_size(s_buff *b);

#endif /* !BUFFER_INTERNAL_H_ */
