
#include "variables/Variable.h"
#include "functions/functions.h"

#ifndef NELGSCRIPT_PARSER_H
#define NELGSCRIPT_PARSER_H

typedef struct Within
{
	void * from;
	enum
	{
		inUndef,
		inComment,
		inWhile,
		inIf,
		inFunction,
	} type;
	unsigned int line;
	
} Within;

Within * newWithin(Within * from);
Within * delWithin(Within * from);

Variable * processType(char * str, int * p);
Variable ** processList(char * str, int * p, char until);

Variable * processVariable(char * str, int * p);
Variable * processFunction(char * line, int * p);

bool processLine(char * line, unsigned int * num);





#endif