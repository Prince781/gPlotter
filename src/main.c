#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "function.h"

int main(int argc, char *argv[]) {
	char *str;
	double val;
	function *f;

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
