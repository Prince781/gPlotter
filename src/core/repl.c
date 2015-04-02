#include <stdio.h>
#include <stdlib.h>
#include <string.h>	/* strcmp, strdup */
#include <readline/readline.h>
#include <readline/history.h>
#include <search.h>	/* t{search,find} etc */
#include "repl.h"
#include "function.h"
#include "variable.h"
#include "util/u_string.h"	/* get_word */
#include "util/console.h"	/* console colors */
#include "program.h"

static int repl_initialized = 0;

enum repl_state {
	INIT,
	EVAL,
	EXIT
};

static struct {
	enum repl_state state;
} repl = {
	.state = INIT
};

static void commands_init(void);
static void commands_uninit(void);

#define cexec(cmd) \
	(cmd->nargs == 0 ? (f_0)cmd->exec : \
	(cmd->nargs == 1 ? (f_1)cmd->exec : \
	(cmd->nargs == 2 ? (f_2)cmd->exec : cmd->exec)))

typedef struct command command;

struct command {
	char *name;
	size_t nargs;
	void (*exec)(command *, void **);
	char *syntax;	/* syntax info */
	char *info;	/* a description (for help command) */
};

static void command_display(const command *c);

static void _cmd_about(command *, void **args);
static void _cmd_eval(command *, void **args);
static void _cmd_exit(command *, void **args);
static void _cmd_dbg(command *, void **args);
static void _cmd_defs(command *, void **args);
static void _cmd_help(command *, void **args);
static void _cmd_set(command *, void **args);

// for _cmd_defs
static void _cmd_defs_fvisit(const void *nodep,
			     const VISIT which,
			     const int depth);
static void _cmd_defs_vvisit(const void *nodep,
			     const VISIT which,
			     const int depth);

static const command default_cmds[] = {
	{ "about", 0, &_cmd_about, "", "display information about "PROGRAM_NAME },
	{ "debug", 1, &_cmd_dbg, "[on|off]", "turn debugging on or off" },
	{ "defined", 0, &_cmd_defs, "", "list all defined functions and variables" },
	{ "eval", 0, &_cmd_eval, "", "begins eval-mode" },
	{ "exit", 0, &_cmd_exit, "", "exits this program" },
	{ "help", 1, &_cmd_help, "[cmd]", "show help" },
	{ "set", 1, &_cmd_set, "[expr]", "assigns a variable or function" }
};

static const size_t num_cmds = sizeof(default_cmds) / sizeof(default_cmds[0]);

void *command_list = NULL;

static int cmd_compare_by_name(const void *c1, const void *c2);

static void commands_init(void) {
	const command *c = &default_cmds[0];
	void *cval;
	size_t len = num_cmds;

	for (; len > 0; --len, ++c) {
		cval = tsearch(c, &command_list, cmd_compare_by_name);
		if ((*(command **)cval) != c)
			fprintf(stderr, "%s: could not insert %s\n",
				__func__, *(char **)c);
	}
}

static command *commands_find(const char *name) {
	void *found;
	char *name2 = strdup(name);
	command cmd = { name2, 0, NULL, NULL };
	found = tfind(&cmd, &command_list, cmd_compare_by_name);
	free(name2);
	return (found != NULL ? *(command **) found : NULL); 
}

static void commands_uninit(void) {
	/* TODO */
}

static void command_display(const command *c) {
	printf(" usage: %s %s - %s\n", c->name, c->syntax, c->info);
}

static void _cmd_about(command *cmd, void **args) {
	printf(" About this program:\n"
		"\t%s, version %s\n"
		"\tbuilt on %s at %s\n",
		PROGRAM_NAME, GPLOTTER_VERSION, __DATE__, __TIME__);
}

static void _cmd_dbg(command *cmd, void **argv) {
	if (argv[0] == NULL)
		command_display(cmd);
	else if (strcmp("on", argv[0]) == 0) {
		function_dbg = 1;
		printf(" %s: turning debugging on\n", cmd->name);
	} else if (strcmp("off", argv[0]) == 0) {
		function_dbg = 0;
		printf(" %s: turning debugging off\n", cmd->name);
	} else
		fprintf(stderr, " %s: '%s' - not a debug state\n",
			cmd->name, argv[0]);
}

static void _cmd_eval(command *cmd, void **argv) {
	long nl;
	enum repl_state saved = repl.state;

	repl.state = EVAL;
	for (nl=0; repl_prompt_eval(nl) != -1; ++nl)
		;
	repl.state = saved;
	printf("\n");
}

static void _cmd_exit(command *cmd, void **argv) {
	repl.state = EXIT;
}

static void _cmd_defs(command *c, void **args) {
	extern void *defined_funcs, *defined_vars;

	printf(" defined functions:\n");
	twalk(defined_funcs, _cmd_defs_fvisit);
	printf(" defined variables:\n");
	twalk(defined_vars, _cmd_defs_vvisit);
}

static void _cmd_defs_fvisit(const void *nodep,
			     const VISIT which,
			     const int depth) {
	function *func = *(function **) nodep;

	if (which == preorder || which == leaf) {
		printf("\t%s(", func->name);
		for (int i=0; i<func->nvars; ++i)
			if (func->vars == NULL)
				printf("x%d,", i);
			else
				printf("%c,", func->vars[i]);
		printf("\b) -> %s\n", func->descr);
	}
}

static void _cmd_defs_vvisit(const void *nodep,
			     const VISIT which,
			     const int depth) {
	variable *var = *(variable **) nodep;

	if (which == preorder || which == leaf)
		printf("\t%s = %lf\n", var->name, var->val);
}

static void _cmd_help(command *cmd, void **argv) {
	int i;
	command *found;

	if (argv[0] ==  NULL)
		for (i=0; i<num_cmds; ++i)
			command_display(&default_cmds[i]);
	else
		for (; *argv; ++argv) {
			found = commands_find((char *) *argv);
			if (found != NULL)
				command_display(found);
			else
				fprintf(stderr, " %s: '%s' - not a command\n",
					cmd->name, (char *) *argv);
		}
}

static void _cmd_set(command *cmd, void **argv) {
	if (argv[0] == NULL) {
		command_display(cmd);
		return;
	}
	/* TODO: evaluate 'set' command */
}

static int cmd_compare_by_name(const void *c1, const void *c2) {
	return strcmp(((command *)c1)->name, ((command *)c2)->name);
}

void repl_init(void) {
	if (!repl_initialized) {
		variables_init();
		functions_init();
		using_history();
		commands_init();
		repl_initialized = 1;
	}
}

void repl_prompt(const char *pre) {
	char *input;
	char *prefix = NULL;
	char *cmd_word;
	command *c;
	size_t wordlen;
	void *argv[16];

	printf("Starting %s v"KRED"%s"KNRM"...\n", 
		PROGRAM_NAME, GPLOTTER_VERSION);
	if (asprintf(&prefix, "%s%s%s %%%s ", KBLU, pre, KGRN, KNRM) == -1)
		fprintf(stderr, "%s: could not allocate prefix\n", __func__);
	else
		while (repl.state != EXIT 
			&& (input = readline(prefix)) != NULL) {
			if ((cmd_word = get_word(input, &wordlen)) != NULL) {
				if ((c = commands_find(cmd_word)) != NULL) {
					argv[1] = NULL;
					argv[0] = input[wordlen] ? 
							input + wordlen+1 : NULL;
					c->exec(c, argv);
				} else if (*cmd_word)
					fprintf(stderr,
						"'%s' - not a command\n",
						cmd_word);
				free(cmd_word);
			} else
				fprintf(stderr, "unknown command\n");
			free(input);
		}

	free(prefix);
}

int repl_prompt_eval(long nl) {
	char *input;
	double val;
	char *prefix = NULL, *temp = NULL;
	function *func;

	asprintf(&prefix, "eval[%d]: ", nl);
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
		commands_uninit();
		repl_initialized = 0;
	}
}
