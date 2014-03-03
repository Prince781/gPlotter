// gpmath.h
#include <stdio.h>
#include <string.h>

/* Declares a monomial. */
typedef struct monomial {
	float c; // our coefficient
	char v; // our variable name
	float e; // our exponent
} monon;

/* Declares a polynomial. */
typedef struct polynomial {
	const int length;
	const monon *m; // monomials
} polyn;

/**
 * Converts the textual form of an equation into a polynomial.
 */
extern polyn to_polynomial(char *eq);

/**
 * Converts a polynomial into a textual representation of an equation.
 */
extern char *to_equation_text(const polyn poly);
