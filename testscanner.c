//This .c file defines functions in testscanner.h for FSA table + driver
//where driver is the function "void testScan()"

#include <stdio.h>
#include "testscanner.h"

token arr[10];
int linecount = 1;
void testScan(FILE* input)
{
	char str[50];
	int SizeOfToken = 0;
	while (1)
	{
		char next;
		int val = fscanf(input, "%s%c", str, &next);
		if (next == '\n')
			linecount++;
		// parse token.
		StrToToken(val == -1 ? NULL : str);
	}
}
