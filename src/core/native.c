#include "native.h"
#include <math.h>

#define function(name, func, params) { #name, (GCallback) &func, params }

struct native_func natives[] = {
	function("abs", fabs, 1),
	/* trigonometric functions */
	function("sin",		sin,		1),
	function("cos",		cos,		1),
	function("tan",		tan,		1),
	function("asin",	asin,		1),
	function("acos",	acos,		1),
	function("atan",	atan,		1),
	/* other functions */
	function("pow",		pow,		2),
	function("min",		fmin,		2),
	function("max",		fmax,		2),
	function("log",		log,		1),
	function("log10",	log10,		1),
	{ 0, 0, 0 }
};
