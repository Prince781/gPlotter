#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "function.h"
#include "variable.h"

int main(int argc, char *argv[]) {
	char *str;
	double val;
	function *f;

	variables_init();
	functions_init();
	f = function_new("f0", "xy", "x*3 - y/3");
	printf("evaluating test function: %s -> %s\n", f->name, f->descr);
	val = function_veval(f, 10., 6.);	// = 30 - 2 = 28
	printf("val = %lf\n", val);
	function_save(f);
	while ((str = readline("eval: ")) != NULL) {
		f = function_new("f1", NULL, str);
		val = function_eval(f, NULL);
		printf("val = %lf\n", val);
		add_history(str);
		free(str);
		function_destroy(f);
	}
	functions_uninit();
	variables_uninit();
	return 0;
}	
