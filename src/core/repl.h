#ifndef _REPL_H
#define _REPL_H

void repl_init(void);

/**
 * prompt user for input
 * @param nl line number
 * @return -1 for EOF, -2 for error reading, 0 otherwise
 */
int repl_prompt(const char *pre, long nl);

void repl_uninit(void);

#endif	/* _REPL_H */
