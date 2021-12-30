
#include "../global.h"
#include "functions.h"

#include "../core.h"
#include "../variables/variables.h"
#include "../error.h"

// and these are automagically filled out functions.h
// then, you can find the enum id of a predefined function

#define _FUNC(x) #x,
const char * const _functions[] = { FUNCTIONS NULL };
#undef _FUNC


Variable * call_function(func f, Variable * argv[])
{
	Variable ** p; p = argv;
	
	switch(f)
	{
		case func_get:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			
			Variable * var = getVariable(getVarString(*p));
			
			Variable * v = newVar();
			copyVar(v, var);
			return v;
		}
		
		case func_set:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			char * name = getVarString(*(p++));
			
			if(*(p+1)) return error(0, ERR_NUM_ARGS);
			
			Variable * var = newVariable(name);
			if(*p)
			{
				copyVar(var, *p);
			}
			
			Variable * var2 = newVar();
			copyVar(var2, var);
			return var2;
		}
		
		case func_defined:
		{
			Variable * var = newVar();
			setVarBool(var, (bool)getVariable(getVarString(*p)));
			return var;
		}
		
		
		// basic
		case func_same:
		{
			bool v = true;
			Variable * comp = (*p);
			for(p++;(*p)!=NULL;p++)
			{
				if(comp->type != (*p)->type)
				{
					v = false;
					break;
				}
				
				if(strcmp(getVarString(comp), getVarString(*p)) != 0)
				{
					v = false;
					break;
				}
			}
			
			Variable * var = newVar(); setVarBool(var, v);
			return var;
		}
		
		case func_not:
		{
			Variable * v = (*(p++));
			if(!v || *p)
			{
				return error(0, ERR_NUM_ARGS);
			}
			
			Variable * var = newVar(); setVarBool(var, !getVarBool(v));
			return var;
		}
		
		case func_all:
		{
			bool all = true;
			
			for(;(*p)!=NULL;p++)
			{
				if(!getVarBool(*p))
				{
					all = false;
					break;
				}
			}
			
			Variable * var = newVar();
			setVarBool(var, all);
			return var;
		}
		
		case func_some:
		{
			bool all = false;
			
			for(;(*p)!=NULL;p++)
			{
				if(getVarBool(*p))
				{
					all = true;
					break;
				}
			}
			
			Variable * var = newVar();
			setVarBool(var, all);
			return var;
		}
		
		case func_largeLeft:
		case func_largeRight:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM l = getVarNumber(*(p++));
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM r = getVarNumber(*(p++));
			if( *p) return error(0, ERR_NUM_ARGS);
			
			Variable * var = newVar();
			
			switch(f)
			{
				case func_largeLeft:
					setVarBool(var, (l > r));
					break;
				case func_largeRight:
					setVarBool(var, (l < r));
					break;
				default: break;
			}
			
			return var;
		}
		
		case func_add:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			// TODO: support for integer
			// or perhaps remove it entirely and only rely on "Number" type 
			
			VAR_NUM n = getVarNumber(*p);
			
			while(*(++p))
			{
				n += getVarNumber(*p);
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_sub:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			// TODO: support for integer
			
			VAR_NUM n = getVarNumber(*p);
			
			while(*(++p))
			{
				n -= getVarNumber(*p);
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_mult:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			// TODO: support for integer
			
			VAR_NUM n = getVarNumber(*p);
			
			while(*(++p))
			{
				if((*p)->type == Undefined)
				{
					// FIXME: why is this required...
					break;
				}
				
				n *= getVarNumber(*p);
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_div:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*p);
			
			VAR_NUM t;
			while(*(++p))
			{
				if((*p)->type == Undefined)
				{
					// FIXME: why is this required...
					break;
				}
				
				//n /= getVarNumber(*p);
				// yes, I tried dividing by zero, doesn't crash:
				// div(0,0) = -nan
				// div(1,0) = inf
				// under GCC and Clang under Mint/Ubuntu
				
				// extra safety probably can't hurt though
				t = getVarNumber(*p);
				if(t == 0)
				{
					if(n == 0)
					{
						// besides, gets rid of negative NaN...
						n = NAN;
					}
					else
					{
						n = INFINITY;
					}
				}
				else
				{
					n /= t;
				}
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_intdiv:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_INT n = getVarInt(*p);
			VAR_INT t;
			while(*(++p))
			{
				if((*p)->type == Undefined)
				{
					// FIXME: why is this required...
					break;
				}
				
				t = getVarInt(*p);
				
				// dividing by zero DOES crash with integers, however
				if(t == 0)
				{
					// well, integers can't represent NaN or Infinity
					// so we'll just become a Number type again
					Variable * var = newVar();
					if(n == 0)
					{
						setVarNumber(var, NAN);
					}
					else
					{
						setVarNumber(var, INFINITY);
					}
					return var;
				}
				else
				{
					n /= t;
				}
			}
			
			Variable * var = newVar();
			setVarInt(var, n);
			return var;
		}
		
		case func_mod:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*p);
			
			VAR_NUM t;
			while(*(++p))
			{
				t = getVarNumber(*p);
				if(t == 0 && n == 0)
				{
					n = NAN;
				}
				else
				{
					n = fmod(n, t);
				}
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_pow:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*(p++));
			VAR_NUM m = (*p)?getVarNumber(*p):0;
			
			Variable * var = newVar();
			setVarNumber(var, powl(n, m));
			return var;
		}
		
		case func_sqrt:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarNumber(var, sqrtl(getVarNumber(*p)));
			return var;
		}
		
		case func_sin:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarNumber(var, sinl(getVarNumber(*p)));
			return var;
		}
		case func_cos:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarNumber(var, cosl(getVarNumber(*p)));
			return var;
		}
		case func_tan:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarNumber(var, tanl(getVarNumber(*p)));
			return var;
		}
		
		case func_atan2:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*(p++));
			VAR_NUM m = (*p)?getVarNumber(*p):0;
			
			Variable * var = newVar();
			setVarNumber(var, atan2l(n, m));
			return var;
		}
		
		case func_round:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*(p++));
			
			if(*p)
			{
				VAR_NUM m = powl(10, getVarNumber(*p));
				n = roundl(n*m)/m;
			}
			else n = roundl(n);
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_ceil:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*(p++));
			
			if(*p)
			{
				VAR_NUM m = powl(10, getVarNumber(*p));
				n = ceill(n*m)/m;
			}
			else n = ceill(n);
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_floor:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*(p++));
			
			if(*p)
			{
				VAR_NUM m = powl(10, getVarNumber(*p));
				n = floorl(n*m)/m;
			}
			else n = floorl(n);
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_min:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*p);
			
			VAR_NUM t;
			while(*(++p))
			{
				t = getVarNumber(*p);
				if(n > t) n = t;
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_max:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			VAR_NUM n = getVarNumber(*p);
			
			VAR_NUM t;
			while(*(++p))
			{
				t = getVarNumber(*p);
				if(n < t) n = t;
			}
			
			Variable * var = newVar();
			setVarNumber(var, n);
			return var;
		}
		
		case func_pi:
		{
			VAR_NUM m = 1;
			if(*p) m = getVarNumber(*p);
			
			Variable * var = newVar();
			setVarNumber(var, m*PI);
			return var;
		}
		
		case func_e:
		{
			VAR_NUM m = 1;
			if(*p) m = getVarNumber(*p);
			
			Variable * var = newVar();
			setVarNumber(var, m*E);
			return var;
		}
		
		case func_isNumber:
		{
			bool num = (bool)(*p);
			if(num) do
			{
				bool dec = true;
				char * s = getVarString(*p);
				int i = 0; do
				{
					if( dec && s[i] == '.') { dec = false; continue; }
					if(!isNumber(s[i]))
					{
						num = false;
					}
				}
				while( num && s[++i] );
			}
			while( num && (*(++p)) );
			
			Variable * var = newVar();
			setVarBool(var, num);
			return var;
		}
		
		case func_time:
		{
			Variable * var = newVar();
			struct timeval t; gettimeofday(&t, NULL);
			setVarNumber(var, t.tv_sec+(t.tv_usec/1e6));
			return var;
		}
		
		case func_print:
		{
			for(;(*p)!=NULL;p++)
			{
				if(p != argv) printf(" ");
				printf("%s", getVarString(*p));
			}
			printf("\r\n");
			
			return newVar();
		}
		
		case func_int:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarInt(var, getVarInt(*p));
			return var;
		}
		
		case func_float:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarNumber(var, getVarNumber(*p));
			return var;
		}
		
		case func_bool:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarBool(var, getVarBool(*p));
			return var;
		}
		
		case func_str:
		{
			if(!*p) return error(0, ERR_NUM_ARGS);
			Variable * var = newVar();
			setVarString(var, getVarString(*p));
			return var;
		}
		
		case num_functions: break;
	}
	return NULL;
}