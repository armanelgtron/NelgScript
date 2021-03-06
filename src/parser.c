
#include "global.h"
#include "parser.h"

#include "core.h"
#include "error.h"
#include "variables/variables.h"

bool interactive;

Within * newWithin(Within * from)
{
	Within * within = (Within *)malloc(sizeof(Within));
	within->from = from;
	within->type = inUndef;
	return within;
}

Within * delWithin(Within * from)
{
	Within * within = (Within *)from->from;
	free(from);
	return within;
}


Variable * processType(char * str, int * p)
{
	bool done = false;
	Variable * d = newVar();
	int l = strlen(str);
	for(;(*p)<l;++(*p))
	{
		if(str[*p] == '"')
		{
			char string[1024]; string[0] = '\0';
			int i = 0;
			for(++(*p);(*p)<l;++(*p))
			{
				switch(str[*p])
				{
					case '\\':
					{
						char c = str[++(*p)];
						switch(c)
						{
							case 'n': string[i++] = '\r'; c = '\n'; break;
							case 't': c = '\t'; break;
							case 'r': c = '\r'; break;
							case 'b': c = '\b'; break;
							default:
								if(isNumber(c))
								{
									char num[4];
									int i = 0;
									for(;str[*p]&&i<3;++(*p))
									{
										if(str[*p] >= '0' && str[*p] <= '8')
										{
											num[i++] = str[*p];
										}
										else break;
									}
									num[i] = '\0';
									
									--(*p);
									
									c = (char)strtoul(num, NULL, 8);
								}
								break;
						}
						if(c == 0) break;
						string[i++] = c;
						break;
					}
					case '"': done = true; break;
					default:
						string[i++] = str[*p];
				}
				if(!done && ((*p)+1)>=l)
				{
					freeVar(d);
					return error((*p)+1, ERR_EOL);
				}
				if(done)
				{
					string[i] = '\0';
					setVarString(d, string);
					++(*p);
					break;
				}
			}
		}
		else if( isNumber(str[*p]) || (str[*p] == '-' && isNumber(str[(*p)+1])) )
		{
			char num[1024];
			int i=0;
			bool isInt = true;
			if(str[*p] == '-') num[i++] = str[(*p)++];
			for(;(*p)<l;++(*p))
			{
				if( str[*p] == '.' || isNumber(str[*p]) )
				{
					if(isInt && str[*p] == '.') isInt = false;
					num[i++] = str[*p];
				}
				else break;
			}
			num[i] = '\0';
			//--(*p);
			setVarString(d, num); 
			
			errno = 0;
			
			if(isInt)
				setVarInt(d, getVarInt(d));
			else
				setVarNumber(d, getVarNumber(d));
			
			if(errno == ERANGE && isInt)
				setVarNumber(d, getVarNumber(d));
			
			done = true;
		}
		
		else
		{
			Variable * t;
			//printf("sc %c\n", str[*p]);
			//if(str[*p] == 'l' && *p == 16) return error(*p, "Not really an error");
			if( (t = processFunction(str, p)) )
			{
				freeVar(d);
				d = t;
				done = true;
			}
			else if( last_error.exists ) { freeVar(d); return NULL; }
			else if( (t = processVariable(str, p)) )
			{
				copyVar(d, t);
				done = true;
			}
			else if( last_error.exists ) { freeVar(d); return NULL; }
			else if(str[*p] != ';')
			{
				freeVar(d);
				return error(*p, ERR_BAD_INPUT);
			}
		}
		
		if(done) break;
	}
	return d;
}

Variable ** processList(char * str, int * p, char until)
{
	//Variable * list[64];
	int s = 2;
	Variable ** list = (Variable **)malloc((sizeof(Variable *)*s));
	
	int x = 0;
	int l = strlen(str);
	for(;(*p)<l;++(*p))
	{
		//printf("p=%i c=%c\n", (*p), str[*p]);
		if(str[*p] == until)
		{
			
			break;
		}
		switch(str[*p])
		{
			case ',': break;
			case ' ': break;
			case ';': break; //FIXME
			default:
			{
				//printf("c %c\n", str[*p]);
				Variable * t = processType(str, p);
				if(!t)
				{
					for(--x;x>0;--x)
					{
						freeVar(list[x]);
					}
					free(list);
					
					return NULL;
				}
				//printf("after type: p=%i c=%c\n", (*p), str[*p]);
				
				if((x+1) > s)
				{
					s += 4;
					Variable ** l = (Variable **)realloc(list, (sizeof(Variable *)*s));
					if(!l)
					{
						fprintf(stderr, "allocation failure, argument list too large\n");
						exit(1);
					}
					else list = l;
					//printf("list %i\n",s);
				}
				list[x++] = t;
			}
		}
	}
	list[x] = NULL;
	return list;
}


Variable * processVariable(char * str, int * p)
{
	Variable * d;
	
	for(int i=MAX_VARIABLES-1;i>=0;--i)
	{
		if(variable(i) && (strpos(str+(*p), variable(i)->name) == 0))
		{
			(*p) += strlen(variable(i)->name);
			return variable(i);
		}
	}
	
	return NULL;
}

Variable * processFunction(char * line, int * p)
{
	char * str = line; str += *p;
	int col = *p;

	for(int _x=0;_functions[_x];++_x)
	{
		if(strpos(str, _functions[_x]+5) == 0)
		{
			for((*p)+=strlen(_functions[_x]+5);(*p)<strlen(line);++(*p))
			{
				char c = line[*p]; if((c <= 122 && c >= 97) || (c <= 90 && c >= 65) || (c <= 57 && c >= 48))
				{
					*p -= strlen(_functions[_x]+5);
					break;
				}
				if(line[*p] == '(')
				{
					(*p)++;
					Variable ** list = processList(line, p, ')');
					
					if(!list)
					{
						return NULL;
					}
					
					//printf("%s\n", _functions[_x]);
					Variable * ret = call_function((func)_x, list);
					last_error.col = col;
					
					for(Variable**p=list;(*p)!=NULL;p++)
					{
						freeVar(*p);
					}
					free(list);
					return ret;
				}
			}
		}
	}
	
	//free(str);
	return NULL;
}


bool processLine(char * line, unsigned int * num)
{
	static Within * within = NULL;
	static Within * skipExec = NULL;
	
	int p = 0;
	
	if(line[0] == '\0') return false;
	
	
	if(
		strstr(line, "#") == line
		||
		strstr(line, "//") == line
	)
	{
		return false;
	}
	
	if(strstr(line, "/*") == line)
	{
		within = newWithin(within);
		within->type = inComment;
		within->line = num?(*num):0;
		return false;
	}
	if(within && within->type == inComment)
	{
		if(strpos(line, "*/") >= 0)
		{
			within = delWithin(within);
			p = strpos(line, "*/")+2;
		}
		else
		{
			return false;
		}
	}
	
	if(strcmp(line, "endif") == 0)
	{
		if(!within || within->type != inIf)
		{
			return error(p, "Line with \"endif\", but not in if");
		}
		
		if(skipExec == within) skipExec = NULL;
		within = delWithin(within);
		return true;
	}
	else if(strstr(line, "end") == line)
	{
		if( !within )
		{
			return error(p, "Line with \"end\", but nothing started");
		}
		
		if(!skipExec)
		{
			if(within->type == inWhile)
			{
				*num = within->line;
			}
		}
		
		if(skipExec == within) skipExec = NULL;
		within = delWithin(within);
		return true;
	}
	else if(strstr(line, "if") == line)
	{
		p += 3;
		
		within = newWithin(within);
		within->type = inIf;
		within->line = num?(*num):0;
		
		if(!skipExec)
		{
			Variable * t = processType(line, &p);
			
			if(!t)
			{
				return false;
			}
			
			if(!getVarBool(t))
			{
				skipExec = within;
			}
			
			freeVar(t);
		}
		return false;
	}
	else if(strstr(line, "while") == line)
	{
		p += 6;
		if(!num)
		{
			if(interactive)
			{
				fprintf(stderr, "Loops are not yet supported in interative mode, sorry.\r\n");
			}
			return error(0, "Internal error: Loops can't be done without a returning line number");
		}
		
		within = newWithin(within);
		within->type = inWhile;
		within->line = *num;
		
		if(!skipExec)
		{
			Variable * t = processType(line, &p);
			if(!t) return false;
			
			if(!getVarBool(t))
			{
				skipExec = within;
			}
			
			freeVar(t);
		}
		return false;
	}
	
	if(skipExec)
	{
		return false;
	}
	
	//printf("len = %i\n",(int)strlen(line));
	Variable * r = processType(line, &p);
	
	if(interactive)
	{
		// already better than PHP's interactive mode :P
		printf("%s\r\n", r?getVar(r):"???");
	}
	
	freeVar(r);
	
	return (r != NULL);
}
