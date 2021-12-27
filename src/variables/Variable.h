
#include "../global.h"

#ifndef NELGSCRIPT_VARIABLE_H
#define NELGSCRIPT_VARIABLE_H

#ifdef USE_QUADMATH
	#define VAR_NUM __float128
	#define VAR_NUM_FSTR strtoflt128
	#define VAR_NUM_TSTR quadmath_snprintf
	#define VAR_NUM_STR "%.16Qg"
#else
	#define VAR_NUM long double
	#define VAR_NUM_FSTR strtod
	#define VAR_NUM_TSTR snprintf
	#define VAR_NUM_STR "%.16Lg"
#endif

	#define VAR_INT int64_t
	#define VAR_INT_STR "%" PRId64


typedef struct Variable
{
	char * name;
	
	enum varType
	{
		Undefined,
		Null,
		Bool,
		Int,
		Number,
		String,
	} type;
	
	void * data;
	
} Variable;

Variable * newVar();

void copyVar(Variable * var, Variable * varSrc);

bool checkVar(Variable * var);
void checkVarAuto(Variable * var);

void clearVar(Variable * var);
void clearVarAuto(Variable * var);

void freeVar(Variable * var);

void setVarString(Variable * var, char * data);
void setVarInt(Variable * var, VAR_INT data);
void setVarNumber(Variable * var, VAR_NUM data);
void setVarBool(Variable * var, bool data);

char * getVar(Variable * var); //! get formatted string representation of variable
char * getVarString(Variable * var);
VAR_INT getVarInt(Variable * var);
VAR_NUM getVarNumber(Variable * var);
bool getVarBool(Variable * var);


#endif