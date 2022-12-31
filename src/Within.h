
#ifndef NELGSCRIPT_WITHIN_H
#define NELGSCRIPT_WITHIN_H

typedef struct Within
{
	void * from;
	void * extra;
	void ** args;
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
#endif
