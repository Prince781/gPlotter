// gpmath.h

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
extern polynomial *to_polynomial(const char *eq);

/**
 * Converts a polynomial into a textual representation of an equation.
 */
extern char *to_equation_text(polynomial *poly);
