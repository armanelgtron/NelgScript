
#include "variables.h"

Variable * variables[MAX_VARIABLES];

void initVariables()
{
	for(int i=MAX_VARIABLES-1;i>=0;--i)
	{
		variables[i] = NULL;
	}
}

Variable * variable(size_t v)
{
	if(v > MAX_VARIABLES) return NULL;
	if(v < 0) return NULL;
	
	return variables[v];
}

Variable * getVariable(char * name)
{
	for(int i=MAX_VARIABLES-1;i>=0;--i)
	{
		if(variables[i] && (strcmp(variables[i]->name, name) == 0))
		{
			return variables[i];
		}
	}
	
	return NULL;
}

Variable * newVariable(char * name)
{
	Variable * var = getVariable(name);
	if(!var)
	{
		var = newVar();
		
		for(int i=MAX_VARIABLES-1;i>=0;--i)
		{
			if(!variables[i])
			{
				variables[i] = var;
				break;
			}
		}
		
		var->name = (char *)malloc(sizeof(char)*strlen(name)+1);
		if(var->name)
		{
			strcpy(var->name, name);
		}
		else
		{
			var->name = "";
		}
	}
	
	return var;
}