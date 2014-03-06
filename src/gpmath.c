#define ERRBUFSIZE 30
#include "gpmath.h"
#include <regex.h>

int to_polynomial(const char *eq) {
	polynomial p = {
		.length = 0
	};
	
	regex_t regex;
	regmatch_t pmatch[100]; // the length of our match
	const char *pattern = "([0-9]\\.[0-9]+)";
	
	int res, len;
	char err_buf[ERRBUFSIZE];
	if ((res = regcomp(&regex, pattern, REG_EXTENDED)) != 0) {
		regerror(res, &regex, err_buf, ERRBUFSIZE);
		printf("regcomp: %s\n", err_buf);
		return -1;
	}
	
	res = regexec(&regex, eq, 100, pmatch, 0);
	if (res == REG_NOMATCH) {
		// printf("Input: \"%s\" Query: \"%s\" - No match.\n", eq, pattern);
		return -1;
	}
	
	int matlen = 0;
	for (int i=0; pmatch[i].rm_so != -1; i++) matlen++;
	
	// TODO: regex
	return matlen;
}

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
