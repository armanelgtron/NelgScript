
#include "Variable.h"

#ifndef NELGSCRIPT_VARIABLES_H
#define NELGSCRIPT_VARIABLES_H

void initVariables();

Variable * variable(size_t v);

Variable * getVariable(char * name);
Variable * newVariable(char * name);

#endif