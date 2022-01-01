
#include "global.h"
#include "error.h"
#include "variables/Variable.h"

struct Error last_error;

void * error(unsigned int col, char * error)
{
	last_error.exists = true;
	last_error.col = col;
	strncpy(last_error.desc, error, 128);
	
	#ifdef ERR_C_BT
		void * bt[10];
		size_t size = backtrace(bt, 10);
		backtrace_symbols_fd(bt, size, STDERR_FILENO);
		fprintf(stderr, "\r\n");
	#endif
	
	#ifdef ERR_SIG
		fprintf(stderr, "Error: %s\r\n", error);
		raise(SIGINT);
	#endif
	
	return NULL;
}

static void printError(char * line)
{
	fprintf(stderr, "Error: %s on line %i col %i\r\n\n", last_error.desc, last_error.line, last_error.col);
	fprintf(stderr, "\t%s\r\n\t%*c^\n", line, last_error.col, last_error.col?' ':'\0');
}

bool isError(char * line, unsigned int lnum)
{
	if(last_error.exists)
	{
		// set up and display error
		last_error.line = lnum;
		printError(line);
		
		// reset error
		last_error.exists = false;
		
		return true;
	}
	return false;
}