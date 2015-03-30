#ifndef _VARIABLE_H
#define _VARIABLE_H
struct variable {
	char *name;
	double val;
	int destroy;	/* whether or not to free() this structure */
};

typedef struct variable variable;

void variables_init(void);

int variable_save(const char *name, double val);

int variable_value_c(char c, double *valptr);

int variable_value(const char *name, double *valptr);

void variables_uninit(void);

#endif /* _VARIABLE_H */
