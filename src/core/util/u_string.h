#ifndef _UTIL_STRING_H
#define _UTIL_STRING_H
#include <stddef.h>

/**
* @brief reverses a string in place
*
* @param str
*
* @return 
*/
char *strrev(char *str);

/**
 * returns the next word from str on
 * modifies *len
 * returns a newly-allocated string, even if *len=0
 * must be freed
 */
char *get_word(const char *str, size_t *len);

#endif
