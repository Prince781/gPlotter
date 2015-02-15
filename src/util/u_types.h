#ifndef _U_TYPES_H
#define _U_TYPES_H

#define type_name(type) _Generic((type), \
int: int, unsigned int: unsigned_int, long: long, \
unsigned long: unsigned_long, double: double, long double: long_double, \
void *: void_ptr)

#endif
