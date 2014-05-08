#define MONOBUFFSIZE 100
#include "gpmath.h" // gpmath
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

polynomial *to_polynomial(const char *eq) {
	polynomial *p = malloc(sizeof(polynomial));
	monomial monos[MONOBUFFSIZE]; // buffered monomials (maximum)

	int m;
	// monos[0] = malloc(sizeof(monomial));
	for (m=0; m<MONOBUFFSIZE && 
		sscanf(eq, "%f%c^%f", &monos[m].coef, &monos[m].var, &monos[m].exp) != EOF;
	m++);
	
	monomial nmonos[m]; // copy monomials
	int i;
	for (i=0; i<m; i++) {
		// nmonos[i] = malloc(sizeof(monomial));
		nmonos[i].coef = monos[i].coef;
		nmonos[i].var = monos[i].var;
		nmonos[i].exp = monos[i].exp;
	}
	
	p->length = m;
	p->monos = nmonos;
	
	return p;
}

char *to_equation_text(polynomial *poly) {
	char *eq = malloc(7 + poly->length * (7+1+1+7+1));

	strcpy(eq, "f(x) = ");

	for (int i=0; i<poly->length; i++) {
		char temp[7+1+1+7]; //float char ^ float
		sprintf(temp, "%3.3f%c^%3.3f", poly->monos[i].coef, 
				poly->monos[i].var, poly->monos[i].exp);
		strcat(eq, temp);
		if (i < poly->length-1) strcat(eq, " + "); // [space] + [space]
	}
	
	return eq;
}
