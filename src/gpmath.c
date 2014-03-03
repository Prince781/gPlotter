#include "gpmath.h"
#include <stdlib.h>

char *to_equation_text(polyn poly) {
	char *eq = malloc(poly.length * 300);

	strcpy(eq, "f(x) = ");
	for (int i=0; i < poly.length; i++) {
		char temp[10];
		sprintf(temp, "%f%c^%f", poly.m[i].c, poly.m[i].v, poly.m[i].e);
		strcat(eq, temp);
	}
	
	return eq;
}
