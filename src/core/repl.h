#ifndef _REPL_H
#define _REPL_H

#define REPL_STATE_MAIN	0x00
#define REPL_STATE_EVAL	0x01

void repl_init(void);

/*
 * starts REPL loop
 */
void repl_prompt(const char *pre);

/**
 * prompt user for evaluating input
 * @param nl line number
 * @return -1 for EOF, -2 for error reading, 0 otherwise
 */
int repl_prompt_eval(long nl);

void repl_uninit(void);

#endif	/* _REPL_H */
