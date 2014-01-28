/*
 * File: itoa_base.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: convert a number to a string
 *
 */

#ifndef ITOA_BASE_H_
# define ITOA_BASE_H_

# include <stddef.h>

size_t itoa_base(int n, char *s, int base);
size_t uitoa_base(unsigned int n, char *s, int base);

#endif /* !ITOA_BASE_H_ */
