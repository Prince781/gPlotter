#ifndef _STACK_H
#define _STACK_H

#define STACK_MINSIZE	10

#define STACK_DEFINE(type, name) \
struct stack_##name { \
	type *data; \
	int top;	/* position in stack */ \
	int maxsize;	/* maximum size of stack */ \
}; \
\
/* maxsize is initial; stack can grow if necessary */ \
struct stack_##name *stack_##name##_new(int maxsize); \
\
void stack_##name##_push(struct stack_##name *s, type elem); \
\
type stack_##name##_pop(struct stack_##name *s); \
\
type stack_##name##_peek(const struct stack_##name *s); \
\
type stack_##name##_get(const struct stack_##name *s, int index); \
\
void stack_##name##_reverse(struct stack_##name *s); \
\
void stack_##name##_destroy(struct stack_##name *s);

#define stack_empty(s) ((s)->top < 0)

#define stack_full(s) ((s)->top >= (s)->maxsize - 1)

#define stack_size(s) ((s)->top + 1)

STACK_DEFINE(int, int)
STACK_DEFINE(unsigned, unsigned)
STACK_DEFINE(long, long)
STACK_DEFINE(unsigned long, unsigned_long)
STACK_DEFINE(double, double)
STACK_DEFINE(long double, long_double)
STACK_DEFINE(void *, void_ptr)

#define stack_push(s, item) _Generic((s), \
struct stack_int *: stack_int_push, \
struct stack_unsigned *: stack_unsigned_push, \
struct stack_long *: stack_long_push, \
struct stack_unsigned_long *: stack_unsigned_long_push, \
struct stack_double *: stack_double_push, \
struct stack_long_double *: stack_long_double_push, \
struct stack_void_ptr *: stack_void_ptr_push)(s, item)

#define stack_pop(s) _Generic((s), \
struct stack_int *: stack_int_pop, \
struct stack_unsigned *: stack_unsigned_pop, \
struct stack_long *: stack_long_pop, \
struct stack_unsigned_long *: stack_unsigned_long_pop, \
struct stack_double *: stack_double_pop, \
struct stack_long_double *: stack_long_double_pop, \
struct stack_void_ptr *: stack_void_ptr_pop)(s)

#define stack_peek(s) _Generic((s), \
struct stack_int *: stack_int_peek, \
struct stack_unsigned *: stack_unsigned_peek, \
struct stack_long *: stack_long_peek, \
struct stack_unsigned_long *: stack_unsigned_long_peek, \
struct stack_double *: stack_double_peek, \
struct stack_long_double *: stack_long_double_peek, \
struct stack_void_ptr *: stack_void_ptr_peek)(s)

#define stack_get(s, i) _Generic((s), \
struct stack_int *: stack_int_get, \
struct stack_unsigned *: stack_unsigned_get, \
struct stack_long *: stack_long_get, \
struct stack_unsigned_long *: stack_unsigned_long_get, \
struct stack_double *: stack_double_get, \
struct stack_long_double *: stack_long_double_get, \
struct stack_void_ptr *: stack_void_ptr_get)(s, i)

#define stack_reverse(s) _Generic((s), \
struct stack_int *: stack_int_reverse, \
struct stack_unsigned *: stack_unsigned_reverse, \
struct stack_long *: stack_long_reverse, \
struct stack_unsigned_long *: stack_unsigned_long_reverse, \
struct stack_double *: stack_double_reverse, \
struct stack_long_double *: stack_long_double_reverse, \
struct stack_void_ptr *: stack_void_ptr_reverse)(s)

#define stack_destroy(s) _Generic((s), \
struct stack_int *: stack_int_destroy, \
struct stack_unsigned *: stack_unsigned_destroy, \
struct stack_long *: stack_long_destroy, \
struct stack_unsigned_long *: stack_unsigned_long_destroy, \
struct stack_double *: stack_double_destroy, \
struct stack_long_double *: stack_long_double_destroy, \
struct stack_void_ptr *: stack_void_ptr_destroy)(s)
#endif	/* _STACK_H */
