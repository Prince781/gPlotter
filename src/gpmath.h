#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	float coef; // coefficient
	char var; // variable name
	float exp; // exponent
} monomial;

typedef struct {
	int length;
	monomial *monos; // the monomials that make up this polynomial
} polynomial;

/**
 * Converts the textual form of an equation into a polynomial.
 */
polynomial to_polynomial(char *eq);

/**
 * Converts a polynomial into a textual representation of an equation.
 */
char *to_equation_text(polynomial poly);
