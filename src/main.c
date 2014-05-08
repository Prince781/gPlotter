/************************************************
 * GPlotter - version 0.1 alpha
 * A neat little program for displaying the
 * output of functions in various ways, among
 * them, as a representation along a Cartesian
 * plane in two-dimensional space.
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * 2014 Princeton Ferro
*************************************************/

#include "gpmath.h"
#include "window.h"

int main(int argc, char **argv) {
	// TODO: add GTK 3.11 code here
	
	// f(x) = x^2 + 3x^1
	monomial ms[2] = {
		{ 1, 'x', 2 },
		{ 3, 'x', 1 }
	};
	
	// test equations
	polynomial *p = malloc(sizeof(polynomial));
	p->length = 2;
	p->monos = ms;
	
	char *eq = to_equation_text(p);
	
	printf("Equation: %s\n", eq);
	
	// test conversion back to polynomial
	
	polynomial *poly = to_polynomial(eq);
	
	int i;
	printf("Converted equation: ");
	for (i=0; i<2; i++, printf(" + "))
		printf("%3.3f%c^%3.3f", poly->monos[i].coef, poly->monos[i].var,
			poly->monos[i].exp);
	printf("\n");

	// initialize GUI:
	int *s = gtkapp_initialize(argc, argv);
	//gtkapp_activate();
	
	return *s;
}