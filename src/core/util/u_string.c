#include "u_string.h"	/* util */
#include <string.h>
#include <ctype.h>	/* isalnum */

char *strrev(char *str)
{
	char *ptr = str;
	char *end = str + strlen(str)-1;
	char tmp;
	
	while (end > str) {
		tmp = *str;
		*str = *end;
		*end = tmp;
		++str;
		--end;
	}

	return ptr;
}

char *get_word(const char *str, size_t *len)
{
	char *cpy = strdup(str);

	for (*len=0; isalnum(cpy[*len]) || cpy[*len] == '_'; ++*len)
		;
	cpy[*len] = '\0';
	return cpy;
}
