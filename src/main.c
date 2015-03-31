#include "core/repl.h"

int main(int argc, char *argv[]) {
	long nl;
	repl_init();
	
	for (nl=0; repl_prompt("eval", nl) != -1; ++nl)
		;

	repl_uninit();
}	
