#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "core/repl.h"
#include "gui/gplotter_app.h"

static struct option opts[] = {
	{ "repl", no_argument, 0, 'r' },
	{ "help", no_argument, 0, 'h' },
	{ 0, 0, 0, 0 }
};

static const char *descrs[] = {
	"start the read-eval-print-loop (REPL)",
	"display help"
};

void print_help(void);

int main(int argc, char *argv[]) {
	int c, i;

	while ((c = getopt_long(argc, argv, "rh", opts, &i)) != -1)
		switch(c) {
		case 'r':
			repl_init();
			repl_prompt("gPlotter");
			repl_uninit();
			return 0;
			break;
		case 'h':
			print_help();
			break;
		default:
			print_help();
			break;
		}
	return g_application_run(G_APPLICATION(gplotter_app_new()), argc, argv);
}

void print_help(void) {
	struct option *opt, *start = &opts[0];

	printf("options:\n");
	for (opt = &opts[0]; opt->name; ++opt)
		printf(" [--%s|-%c] : %s\n", opt->name, 
			opt->val, descrs[opt-start]);
	exit(1);
}
