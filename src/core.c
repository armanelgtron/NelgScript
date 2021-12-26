
#include "global.h"

int strpos(const char * str, const char * tofind)
{
	char * p = strstr(str, tofind);
	if(p) return p - str;
	return -1;
}

void strtrim(char * str)
{
	// FIXME
	// ...actually, what was I fixing again? it seems to be working...
	
	int x = 0, i = 0;
	int l = strlen(str);
	for(;i<l;++i)
	{
		if(str[i] != ' ' && str[i] != '\t')
		{
			break;
		}
	}
	if(i)
	{
		for(;i<l&&x<1024;++i)
		{
			//printf("%i %i\n", i, x);
			str[x++] = str[i];
		}
		str[x] = '\0';
	}
	if( (l = strlen(str)) )
	{
		for(i=l-1;i>=0;--i)
		{
			if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			{
				break;
			}
		}
		str[i+1] = '\0';
	}
}

int strtimes(const char * str, char c)
{
	int t = 0;
	for(int i=strlen(str)-1;i>=0;--i)
	{
		if(str[i] == c) ++t;
	}
	return t;
}