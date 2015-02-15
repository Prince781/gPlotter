#include "u_string.h"	/* util */
#include <string.h>

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
