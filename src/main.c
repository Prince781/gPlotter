#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "function.h"

int main(int argc, char *argv[]) {
	char *str;
	double val;
	function f;

	while ((str = readline("eval: ")) != NULL) {
		f.descr = str;
		f.len = strlen(f.descr);
		val = function_eval(&f);
		printf("val = %lf\n", val);
		free(str);
	}

	return 0;
}	
