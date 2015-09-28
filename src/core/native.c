#include "native.h"
#include <math.h>

#define function(func, params) { #func, (GCallback) &func, params }
#define function_custom(name, func, params) { name, (GCallback) &func, params }

struct native_func natives[] = {
	function_custom("abs",	fabs, 	1),
	/* trigonometric functions */
	function(sin,	1),
	function(cos,	1),
	function(tan,	1),
	function(asin,	1),
	function(acos,	1),
	function(atan,	1),
	/* other functions */
	function(log,	1),
	function(log10,	1),
	function(exp,	2),
	function_custom("min",	fmin,	2),
	function_custom("max",	fmax,	2),
	{ 0, 0, 0 }
};
