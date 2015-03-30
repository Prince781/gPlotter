#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>	/* strcmp */
#include "variable.h"

static int __variable_save(const variable *v);

static int var_compare_by_name(const void *v1, const void *v2);

static const variable default_vars[] = {
	{ "e", M_E, 0 },
	{ "pi", M_PI, 0 }
};

static void *defined_vars = NULL;

void variables_init(void) {
	const variable *v = &default_vars[0];
	size_t len = sizeof(default_vars) / sizeof(default_vars[0]);

	for (; len > 0; --len, ++v)
		__variable_save(v);
}

int variable_save(const char *name, double val) {
	variable *v;

	if ((v = malloc(sizeof(*v))) == NULL)
		return 0;
	v->name = strdup(name);
	v->val = val;
	return __variable_save(v);
}

static int __variable_save(const variable *v) {
	void *vval = tsearch(v, &defined_vars, var_compare_by_name);
	if ((*(variable **) vval) != v) {
		fprintf(stderr, "%s: Could not insert %s\n", 
			__func__, v->name);
		return 0;
	}
	return 1;
}

int variable_value_c(char c, double *valptr) {
	char buf[2];

	buf[0] = c;
	buf[1] = '\0';

	return variable_value(buf, valptr);
}

int variable_value(const char *name, double *valptr) {
	char *name2 = strdup(name);
	void *found;
	variable var = { name2, 0, 0 };
	
	found = tfind(&var, &defined_vars, var_compare_by_name);
	free(name2);
	if (found != NULL) {
		*valptr = (*(variable **) found)->val;
		return 1;
	}
	return 0;
}

static int var_compare_by_name(const void *v1, const void *v2) {
	return strcmp(((variable *)v1)->name, ((variable *)v2)->name);
}

void variables_uninit(void) {
	/* TODO */
}
