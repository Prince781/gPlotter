#include "gpmath.h"
#include <regex.h>

/*
polynomial to_polynomial(const char *eq) {
	polynomial p = {
		.length = NULL,
		.monos = NULL
	};
	regex_t regex;
	int reti = regcomp(&regex, "((\d[\.]?\d*)[A-Za-z][\^](\d[\.]?\d*))", 0);
	if (reti) return p; // regex compilation failed
	// TODO: regex
}

*/

char *to_equation_text(polynomial *poly) {
	char *eq = malloc(7 + poly->length * (7+1+1+7+1));

	strcpy(eq, "f(x) = ");

	for (int i=0; i < poly->length; i++) {
		char temp[7+1+1+7]; //float char ^ float
		sprintf(temp, "%3.3f%c^%3.3f", poly->monos[i].coef, poly->monos[i].var, poly->monos[i].exp);
		strcat(eq, temp);
		if (i < poly->length-1) strcat(eq, " + "); // [space] + [space]
	}
	
	return eq;
}
