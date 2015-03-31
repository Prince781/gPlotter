#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "repl.h"
#include "function.h"
#include "variable.h"

static int repl_initialized = 0;

void repl_init(void) {
	if (!repl_initialized) {
		variables_init();
		functions_init();
		using_history();
		repl_initialized = 1;
	}
}

int repl_prompt(const char *pre, long nl) {
	char *input;
	double val;
	char *prefix = NULL, *temp = NULL;
	function *func;

	asprintf(&prefix, "%s[f%d]: ", pre, nl);
	if ((input = readline(prefix)) != NULL) {
		asprintf(&temp, "f%d", nl);
		func = function_new(temp, NULL, input);
		val = function_eval(func, NULL);
		printf("result: %lf\n", val);
		add_history(input);
		function_destroy(func);
		free(input);
		free(prefix);
		free(temp);
	} else
		free(prefix);

	return input != NULL ? 0 : -1;
}

void repl_uninit(void) {
	if (repl_initialized) {
		functions_uninit();
		variables_uninit();
		clear_history();
		repl_initialized = 0;
	}
}
