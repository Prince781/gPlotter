#include "core/repl.h"

int main(int argc, char *argv[]) {
	repl_init();
	repl_prompt("gplotter");
	repl_uninit();
}	
