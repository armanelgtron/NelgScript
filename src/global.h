
#ifndef NELGSCRIPT_GLOBAL_H
#define NELGSCRIPT_GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>

#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <limits.h>
#include <inttypes.h>

#include <unistd.h>
#ifdef WIN32
	// FIXME: completely untested, and I think io.h no longer is a thing
	#include <io.h>
	#define isatty _isatty
	#define fileno _fileno
#endif

#ifdef ERR_C_BT
	#include <execinfo.h>
#endif

#ifdef ERR_SIG
	#include <signal.h>
#endif

#ifdef USE_QUADMATH
	#include <quadmath.h>
	#define PI M_PIq
	#define E M_Eq
#else
	#define PI M_PI
	#define E M_E
#endif

//#define fopen_ fopen64
#define fopen_ fopen

#define MAX_VARIABLES 256
#define MAX_FUNCTIONS 128

extern bool interactive;

#endif