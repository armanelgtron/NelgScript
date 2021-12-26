
#include "variables/Variable.h"

#ifndef NELGSCRIPT_ERROR_H
#define NELGSCRIPT_ERROR_H

struct Error {
	bool exists;
	unsigned int line, col;
	char desc[128];
} last_error;

#define ERR_NUM_ARGS   "Wrong number of arguments"
#define ERR_EOL        "Unexpected end of line"
#define ERR_BAD_INPUT  "Unknown input"

void * error(unsigned int col, char * error); //! create an error. Returns NULL for convenience.
bool isError(char * line, unsigned int lnum); //! Check for error, automatically print it and clear it.

#endif