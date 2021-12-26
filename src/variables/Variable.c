
#include "Variable.h"

Variable * newVar()
{
	Variable * var = (Variable *)malloc(sizeof(Variable));
	if(var)
	{
		var->name = "";
		var->type = Undefined;
		var->data = NULL;
	}
	return var;
}


bool checkVar(Variable * var)
{
	if(!var) return false;
	if(!var->data)
	{
		switch(var->type)
		{
			case Undefined:
			case Null:
				break;
			default:
				return false;
		}
	}
	return true;
}

void checkVarAuto(Variable * var)
{
	if(var && !checkVar(var)) var->type = Null;
}

void clearVar(Variable * var)
{
	free(var->data);
	var->data = NULL;
	var->type = Undefined;
}

void clearVarAuto(Variable * var)
{
	//if(var->type == String) return;
	if(var->type == Bool) return;
	
	clearVar(var);
}

void freeVar(Variable * var)
{
	if(!var) return;
	
	clearVarAuto(var);
	free(var);
}

void setVarString(Variable * var, char * data)
{
	if(var->data) clearVarAuto(var);
	
	var->type = String;
	var->data = malloc(sizeof(char)*strlen(data)+1);
	
	checkVarAuto(var);
	
	if(var->data)
	{
		sprintf(((char *)var->data), "%s", data);
	}
}
void setVarInt(Variable * var, VAR_INT data)
{
	if(var->data) clearVarAuto(var);
	
	var->type = Int;
	var->data = malloc(sizeof(VAR_INT));
	
	checkVarAuto(var);
	
	if(var->data)
	{
		*((VAR_INT *)var->data) = data;
	}
}

void setVarNumber(Variable * var, VAR_NUM data)
{
	if(var->data) clearVarAuto(var);
	
	var->type = Number;
	var->data = malloc(sizeof(VAR_NUM));
	
	checkVarAuto(var);
	
	if(var->data)
	{
		*((VAR_NUM *)var->data) = data;
	}
}

void setVarBool(Variable * var, bool data)
{
	if(var->data) clearVarAuto(var);
	
	var->type = Bool;
	var->data = (void *)data;
}



char * getVarString(Variable * var)
{
	char * ret;
	static char * ret2 = NULL;
	if(!ret2) ret2 = (char *)malloc(sizeof(char)*64);
	switch(var->type)
	{
		case Undefined:
			ret = "undefined";
			break;
		case Null:
			ret = "null";
			break;
		case String:
			ret = var->data;
			break;
		case Int:
			sprintf(ret2, VAR_INT_STR, *((VAR_INT *)var->data));
			ret = ret2;
			break;
		case Number:
			VAR_NUM_TSTR(ret2, 63, VAR_NUM_STR, *((VAR_NUM *)var->data));
			ret = ret2;
			break;
		case Bool:
			ret = var->data?"true":"false";
			break;
	}
	return ret;
}

char * getVar(Variable * var)
{
	char * ret;
	static char * ret2 = NULL;
	switch(var->type)
	{
		case String:
		{
			if(ret2) free(ret2);
			ret2 = malloc(sizeof(char)*(strlen(var->data)+3)); //+strtimes(var->data, '"')
			sprintf(ret2, "\"%s\"", ((char *)var->data));
			ret = ret2;
			break;
		}
		default:
			ret = getVarString(var);
	}
	return ret;
}

VAR_INT getVarInt(Variable * var)
{
	VAR_INT ret;
	switch(var->type)
	{
		case Undefined:
		case Null:
			ret = 0;
			break;
		case String:
			ret = strtol(((char *)var->data), '\0', 0);
			break;
		case Int:
			ret = *((VAR_INT *)var->data);
			break;
		case Number:
			ret = *((VAR_NUM *)var->data);
			break;
		case Bool:
			ret = var->data?1:0;
			break;
	}
	return ret;
}

VAR_NUM getVarNumber(Variable * var)
{
	VAR_NUM ret;
	switch(var->type)
	{
		case Undefined:
		case Null:
			ret = 0;
			break;
		case String:
			ret = VAR_NUM_FSTR(((char *)var->data), '\0');
			break;
		case Int:
			ret = *((VAR_INT *)var->data);
			break;
		case Number:
			ret = *((VAR_NUM *)var->data);
			break;
		case Bool:
			ret = var->data?1:0;
			break;
	}
	return ret;
}

bool getVarBool(Variable * var)
{
	return getVarInt(var)?true:false;
}


void copyVar(Variable * var, Variable * varSrc)
{
	switch(varSrc->type)
	{
		case Undefined:
		case Null:
			clearVarAuto(var);
			var->type = varSrc->type;
			break;
		case String:
			setVarString(var, varSrc->data);
			break;
		case Int:
			setVarInt(var, *((VAR_INT *)(varSrc->data)) );
			break;
		case Number:
			setVarNumber(var, *((VAR_NUM *)(varSrc->data)) );
			break;
		case Bool:
			setVarBool(var, varSrc->data);
			break;
	}
}