#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_NEW_DEFINE(type, name) \
static void stack_##name##_grow(struct stack_##name *s, int addition); \
struct stack_##name *stack_##name##_new(int maxsize) \
{ \
	struct stack_##name *s; \
\
	if ((s = malloc(sizeof(struct stack_##name))) == NULL) { \
		fprintf(stderr, "%s: could not create stack\n", __func__); \
		return NULL; \
	} \
	s->data = malloc(maxsize * sizeof(type)); \
	s->maxsize = maxsize; \
	s->top = -1; \
\
	return s; \
}

#define STACK_PUSH_DEFINE(type, name) \
void stack_##name##_push(struct stack_##name *s, type elem) \
{ \
	if (stack_full(s)) \
		stack_##name##_grow(s, s->maxsize); \
	s->data[++s->top] = elem; \
}

#define STACK_POP_DEFINE(type, name) \
type stack_##name##_pop(struct stack_##name *s) \
{ \
	if (stack_empty(s)) { \
		fprintf(stderr, "%s: stack is empty\n", __func__); \
		return 0; \
	} else \
		return s->data[s->top--]; \
}

#define STACK_PEEK_DEFINE(type, name) \
type stack_##name##_peek(const struct stack_##name *s) \
{ \
	if (stack_empty(s)) { \
		fprintf(stderr, "%s: stack is empty\n", __func__); \
		return 0; \
	} else \
		return s->data[s->top]; \
}

#define STACK_GET_DEFINE(type, name) \
type stack_##name##_get(const struct stack_##name *s, int index) \
{ \
	if (index < 0 || index > s->top) { \
		fprintf(stderr, "%s: %d is out of range\n", __func__, index); \
		return 0; \
	} else \
		return s->data[index]; \
}

#define STACK_GROW_DEFINE(type, name) \
static void stack_##name##_grow(struct stack_##name *s, int addition) \
{ \
	int nsize = s->maxsize + addition; \
	void *p; \
\
	if ((p = realloc(s->data, nsize * sizeof(*s->data))) == NULL) \
		fprintf(stderr, "%s: could not resize stack (%d => %d)\n", \
			__func__, s->maxsize, nsize); \
	else { \
		s->data = p; \
		s->maxsize = nsize; \
	} \
}

#define STACK_REVERSE_DEFINE(type, name) \
void stack_##name##_reverse(struct stack_##name *s) \
{ \
	type tmp; \
	type *str = s->data; \
	type *end = s->data + s->top; \
\
	while (end > str) { \
		tmp = *str; \
		*str = *end; \
		*end = tmp; \
		++str; \
		--end; \
	} \
}

#define STACK_DESTROY_DEFINE(type, name) \
void stack_##name##_destroy(struct stack_##name *s) \
{ \
	free(s->data); \
	free(s); \
}

#define STACK_DEFINE_FUNCTIONS(type, name) \
STACK_NEW_DEFINE(type, name) \
STACK_PUSH_DEFINE(type, name) \
STACK_POP_DEFINE(type, name) \
STACK_PEEK_DEFINE(type, name) \
STACK_GET_DEFINE(type, name) \
STACK_GROW_DEFINE(type, name) \
STACK_REVERSE_DEFINE(type, name) \
STACK_DESTROY_DEFINE(type, name)

STACK_DEFINE_FUNCTIONS(int, int)
STACK_DEFINE_FUNCTIONS(unsigned, unsigned)
STACK_DEFINE_FUNCTIONS(long, long)
STACK_DEFINE_FUNCTIONS(unsigned long, unsigned_long)
STACK_DEFINE_FUNCTIONS(double, double)
STACK_DEFINE_FUNCTIONS(long double, long_double)
STACK_DEFINE_FUNCTIONS(void *, void_ptr)
