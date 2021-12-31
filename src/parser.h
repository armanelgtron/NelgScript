
#include "variables/Variable.h"
#include "functions/functions.h"

#ifndef NELGSCRIPT_PARSER_H
#define NELGSCRIPT_PARSER_H

typedef struct ParsedData
{
	void * data;
	void ** list;
	enum
	{
		pNull,
		pVariable,
		pFunction,
		pIf,
		pWhile,
	} type;
	unsigned int line, col;

} ParsedData;

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

ParsedData * processType(char * str, int * p);
ParsedData ** processList(char * str, int * p, char until);

Variable * processVariable(char * str, int * p);
ParsedData * processFunction(char * line, int * p);

ParsedData * processLine(char * line, unsigned int * num);

Variable * runParsed(ParsedData * line, unsigned int * num);





#endif