
#include "variables/Variable.h"
#include "functions/functions.h"

#ifndef NELGSCRIPT_PARSER_H
#define NELGSCRIPT_PARSER_H

typedef struct ParsedData
{
	void * data;
	void ** list;
	int num;
	enum
	{
		pNull,
		pVariable,
		pFunction,
		pIf,
		pWhile,
	} type;
	unsigned int line, col;
	char * lineStr;

} ParsedData;

#include "Within.h"

ParsedData * processType(char * str, int * p);
ParsedData ** processList(char * str, int * p, char until);

Variable * processVariable(char * str, int * p);
ParsedData * processFunction(char * line, int * p);

ParsedData * processLine(char * line, unsigned int * num, unsigned int realnum);

Variable * runParsed(ParsedData * line, unsigned int * num);





#endif