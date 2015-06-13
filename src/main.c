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

	while ((c = getopt_long(argc, argv, "rh", opts, &i)) != -1) {
		if (c == 'r') {
			repl_init();
			repl_prompt("gPlotter");
			repl_uninit();
			return 0;
		} else {
			print_help();
		}
	}

	g_autoptr(GPlotterApp) app = gplotter_app_new();
	return g_application_run(G_APPLICATION(app), argc, argv);
}

void print_help(void) {
	struct option *opt, *start = &opts[0];

	printf("options:\n");
	for (opt = &opts[0]; opt->name; ++opt)
		printf(" [--%s|-%c] : %s\n", opt->name, 
			opt->val, descrs[opt-start]);
	exit(1);
}
