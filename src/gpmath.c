#include "gpmath.h"

char *to_equation_text(polynomial poly) {
	char *eq = malloc(7 + poly.length * (7+1+1+7+1));

	strcpy(eq, "f(x) = ");

	for (int i=0; i < poly.length; i++) {
		char temp[7+1+1+7+1]; //float char ^ float [space]
		sprintf(temp, "%3.3f%c^%3.3f ", poly.monos[i].coef, poly.monos[i].var, poly.monos[i].exp);
		strcat(eq, temp);
	}
	
	return eq;
}
