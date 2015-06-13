#include "session.h"
#include <string.h>
#include <stdlib.h>

ssize_t gp_session_add_func(struct gp_session *s, ui_function *f) {
	ptrdiff_t length = s->f_empty - s->ui_f;

	if (length >= s->nfuncs) {
		s->nfuncs *= 2;
		s->ui_f = realloc(s->ui_f, s->nfuncs);
		s->f_empty = s->ui_f + length;
		memset(s->f_empty, 0, length * sizeof(*s->ui_f)); 
	}
	*s->f_empty = f;
	for (; s->f_empty - s->ui_f >= s->nfuncs || *s->f_empty != NULL;
		++s->f_empty);
	
	return s->f_empty - s->ui_f;
}

ui_function *gp_session_remove_func(struct gp_session *s, ssize_t index) {
	ui_function *ptr = NULL;

	if (index >= 0 && index < s->nfuncs && s->ui_f[index] != NULL) {
		ptr = s->ui_f[index];
		s->ui_f[index] = NULL;
		s->f_empty = s->ui_f + index;
	}

	return ptr;
}
