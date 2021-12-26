
#include "../variables/Variable.h"

#ifndef _NELGSCRIPT_FUNCTIONS_H
#define _NELGSCRIPT_FUNCTIONS_H

// based on https://www.purl.org/stefan_ram/pub/c_preprocessor_applications_en
// for now, functions can just be added wherever within this macro, and of course to call_function

#define FUNCTIONS \
	_FUNC(func_get) \
	_FUNC(func_set) \
	_FUNC(func_defined) \
	\
	_FUNC(func_same) \
	_FUNC(func_not) \
	_FUNC(func_all) \
	_FUNC(func_some) \
	\
	_FUNC(func_largeLeft) \
	_FUNC(func_largeRight) \
	\
	_FUNC(func_add) \
	_FUNC(func_sub) \
	_FUNC(func_mult) \
	_FUNC(func_div) \
	_FUNC(func_intdiv) \
	_FUNC(func_mod) \
	\
	_FUNC(func_pow) \
	_FUNC(func_sqrt) \
	\
	_FUNC(func_sin) \
	_FUNC(func_cos) \
	_FUNC(func_tan) \
	\
	_FUNC(func_atan2) \
	\
	_FUNC(func_round) \
	_FUNC(func_ceil) \
	_FUNC(func_floor) \
	\
	_FUNC(func_min) \
	_FUNC(func_max) \
	\
	_FUNC(func_pi) \
	_FUNC(func_e) \
	\
	_FUNC(func_time) \
	\
	_FUNC(func_print) \


#define _FUNC(x) x,
typedef enum { FUNCTIONS num_functions } func;
#undef _FUNC

extern const char * const _functions[];

Variable * call_function(func f, Variable * argv[]); //! calls a predefined function

#endif