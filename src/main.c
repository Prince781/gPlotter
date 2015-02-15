#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "function.h"

int main(int argc, char *argv[]) {
	char *str;
	double val;
	function *f;

	double vs[] = {10, 11, 12};
	f = function_new("x^2 + y^2 + z^2", "xyz");
	printf("evaluating test function: %s\n", (char *)f);
	val = function_eval(f, vs);
	while ((str = readline("eval: ")) != NULL) {
		f = function_new(str, NULL);
		val = function_eval(f, NULL);
		printf("val = %lf\n", val);
		add_history(str);
		free(str);
		function_destroy(f);
	}

	return 0;
}	
