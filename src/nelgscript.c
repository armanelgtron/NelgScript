
#include "global.h"
#include "core.h"
#include "error.h"
#include "variables/Variable.h"
#include "variables/variables.h"
#include "functions/functions.h"
#include "parser.h"

int main(int argc, char ** argv)
{
	initVariables();
	
	last_error.exists = false;
	
	
	interactive = false;
	
	if(isatty(fileno(stdin)))
	{
		interactive = true;
	}
	
	FILE * file = stdin;
	
	char * bin;
	{
		char ** p = argv;
		for(bin = *(p++);(*p)!=NULL;p++)
		{
			if( strcmp(*p, "-") == 0 )
			{
				interactive = false;
				if(file != stdin)
				{
					fclose(file);
				}
				file = stdin;
			}
			else if( strpos(*p, "-") == 0 )
			{
				if(
					strcmp(*p, "--help") == 0
				||	strcmp(*p, "-h") == 0
				)
				{
					printf("--help, -h: show this help text\r\n");
					printf("--version, -v: version\r\n");
					
					return 0;
				}
				else if(
					strcmp(*p, "--version") == 0
				||	strcmp(*p, "-v") == 0
				)
				{
					printf("NelgScript v0 (%s)\r\n", __DATE__);
					printf("%i built-in functions\r\n", num_functions);
					
					return 0;
				}
				else if(
					strcmp(*p, "-a") == 0
				||	strcmp(*p, "-i") == 0
				)
				{
					if(file == stdin)
					{
						interactive = true;
					}
					else
					{
						fprintf(stderr, "Can't make file interactive!\r\n");
						
						return 1;
					}
				}
				else if( strcmp(*p, "--test") == 0 )
				{
					Variable * test = newVar();
					if(test)
					{
						printf("inital value: %s\r\n", getVarString(test));
						checkVarAuto(test);
						printf("checked value: %s\r\n", getVarString(test));
												
						setVarString(test, "3.1415926535879");
						printf("string to string:  %s\n",  getVarString(test));
						
						setVarNumber(test, getVarNumber(test));
						printf("number to string:  %s\n",  getVarString(test));
						
						setVarInt(test, getVarInt(test));
						printf("integer to string: %s\n",  getVarString(test));
						
						setVarBool(test, getVarBool(test));
						printf("boolean to string: %s\n",  getVarString(test));
						
						printf("call print function: ");
						Variable * test2[] = {test, NULL};
						call_function(func_print, test2);
					}
					else
					{
						printf("Unable to create a variable.\r\n");
					}
					freeVar(test);
					return 0;
				}
				else
				{
					printf("Invalid argument, see %s --help\r\n", bin);
					return 1;
				}
			}
			else
			{
				interactive = false;
				if(file != stdin)
				{
					fclose(file);
				}
				file = fopen_(*p, "r");
				if(!file)
				{
					fprintf(stderr, "Couldn't open file %s.", *p);
				}
			}
		}
	}
	
	if(interactive)
	{
		unsigned int fline = 0;
		
		char event[1024];
		printf("NelgScript C edition v0\n");
		printf("> ");
		
		while( !feof( stdin ) && fgets( event, 1024, stdin ) )
		{
			// print if we're no longer a terminal
			if(!isatty(fileno(stdin))) fprintf(stderr, "%s", event);
			
			strtrim(event);
			ParsedData * p = processLine(event, NULL, fline);
			if(p)
			{
				Variable * d = runParsed(p, NULL);
				
				// already better than PHP's interactive mode :P
				printf("%s\r\n", d?getVar(d):"???");
				
				free(p);
				// I think this is safe to free...
				//free(d);
				// it wasn't safe to free
			}
			
			// prints any errors
			isError(event, ++fline);
			
			printf("> ");
		}
		
		return 0;
	}
	
	unsigned int line = 0;
	ParsedData ** lines;
	
	{
		char event[1024]; event[1022] = '\n';
		unsigned int s = 16, o = 0;
		lines = (ParsedData **)malloc(s*sizeof(ParsedData *));
		if(!lines) return 1;
		unsigned int fline = 0;
		while( !feof( file ) && fgets( event, 1023, file ) )
		{
			if(event[1022] != '\n')
			{
				fprintf(stderr, "Warning: Line %i is ridiculously long and will be truncated...\r\n", (line+1));
				event[1022] = '\n'; event[1023] = '\0';
				
				while( !feof( file ) && fgetc(file) != '\n' );
			}
			
			strtrim(event);
			
			unsigned int oline = line, nline = line;
			ParsedData * d = processLine(event, &nline, fline);
			
			if(isError(event, ++fline))
			{
				fprintf(stderr,"\r\n");
				return 1;
			}
			lines[line] = d;
			
			if( d && d->type == pVariable )
			{
				// pointless and we'd free the variable
				free(d);
				d = NULL;
			}
			
			if( d && ((++line) >= s) )
			{
				s *= 2;
				ParsedData ** l = (ParsedData **)realloc(lines, s*sizeof(ParsedData *));
				if(!l)
				{
					fprintf(stderr, "Not enough memory to load in file\r\n");
					return 1;
				}
				lines = l;
			}
		}
		lines[line] = NULL;
	}
	
	//unsigned int linesc = line;
	line = 0;
	while(lines[line])
	{
		unsigned int oline = line, nline = line;
		freeVar(runParsed(lines[line], &nline));
		
		if(isError(lines[line]->lineStr, lines[line]->line))
		{
			fprintf(stderr,"\r\n");
			return 1;
		}
		
		if(nline != oline) line = nline;
		else line++;
	}
	
	return 0;
}