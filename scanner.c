//This .c file tokenizes the input from input file or prints out a token scanner error

#include <string.h>
#include "scanner.h"

//{ NO_tk = 0, IDENT_tk = 1, NUM_tk = 2, KW_tk =3, OPER_tk = 4, COMMENT_tk=5, EOF_tk=6 }
char* tokenNames[] = { "NOToken", "Identifier", "Number", "Keyword", "Operator","COMMENT", "EOF" };

char* keywords[] = { "begin", "end", "loop", "whole", "void", "exit", "getter",
                     "outter", "main", "if", "then", "assign", "data", "proc"
                   };
char* operators[] = { "=", "=>", "=<", "==", ":", ":=", "+", "-", "*", "/", "%",
                      ".", "(", ")", ",", "{", "}", ";", "[", "]"
                    };

int comment_start = 0;
int comment_end = 0;

int checkNumber(char* str)
{
	if (str == NULL)
		return 0;

	if (strlen(str) > 8)
		return 0;

	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return 0;
		str++;
	}
	return 1;
}

int checkKeyword(char* str)
{
	int i;
	for (i = 0; i < 14; i++)
	{
		if (strcmp(keywords[i], str) == 0)
			return 1;
	}
	return 0;
}

int checkOperator(char* str)
{
	int i;
	for (i = 0; i < 20; i++)
	{
		if (strcmp(operators[i], str) == 0)
			return 1;
	}
	return 0;
}

int checkIdentifier(char* str)
{
	if (str == NULL)
		return 0;

	if (strlen(str) > 8)
		return 0;

	if (str[0] == '_' || str[0] >= 'a' && str[0] <= 'z' || str[0] >= 'A' && str[0] <= 'Z') // comment start = 0;
	{
		int i;
		for (i = 1; i < strlen(str); i++)
		{
			if ((str[i] >= 'a' && str[i] <= 'z') ||(str[i] >= 'A' && str[i] <= 'Z') ||(str[i] >= '0' && str[i] <= '9'))
			{
				continue;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

int checkEOF(char* str)
{
	if (str == 0)
		return 1;
	return 0;
}

int checkComment(char* str)
{
	if (str == NULL)
		return 0;

	if (str[0] == '$' && str[1] == '$'&& comment_start == 0)
	{
		comment_start = 1;
	}
	else if(str[0] == '$' && str[1] == '$' && comment_start == 1)
	{
		comment_end = 1;
	}
	// used to check any word inside comment like "$$ 'comment' $$"
	if(comment_start == 1 && comment_end == 0)
	{
		return 1;
	}
	else if(comment_start==1 && comment_end == 1)
	{
		comment_start = 0;
		comment_end = 0;
		return 1;
	}
	return 0;
}

token checkToken(char* str)
{
	token t;
	t.str = str;
	if (checkComment(str))
	{
		t.id = COMMENT_tk;
	}
	else if (checkEOF(str))
	{
		t.id = EOF_tk;
	}
	else if (checkNumber(str))
	{
		t.id = NUM_tk;
	}
	else if (checkKeyword(str))
	{
		t.id = KW_tk;
	}
	else if (checkOperator(str))
	{
		t.id = OPER_tk;
	}
	else if (checkIdentifier(str))
	{
		t.id = IDENT_tk;
	}
	else
	{
		t.id = NO_tk;
	}
	return t;
}


void StrToToken(char * str,token* ArrayToken , int* size)
{
	int x=0,k=0;
	char buffer[50]="";
	int i;
	for(i=0; i<strlen(str); i++)
	{
		// parse identifier
		if (str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z'))
		{
			buffer[k++] = str[i];
			for ( x= i+1; x < strlen(str); x++)
			{
				if ((str[x] >= 'a' && str[x] <= 'z') ||(str[x] >= 'A' && str[x] <= 'Z') ||(str[x] >= '0' && str[x] <= '9'))
				{
					buffer[k++] = str[x];
					i = x;
				}
				else
				{
					i = x-1;
					break;
				}
			}
		}
		// parse Number
		else if(str[i] >= '0' && str[i] <= '9')
        {
            for ( x= i; x < strlen(str); x++)
			{
				if ((str[x] >= '0' && str[x] <= '9')||(str[x] >= 'a' && str[x] <= 'z') ||(str[x] >= 'A' && str[x] <= 'Z'))
				{
					buffer[k++] = str[x];
					i = x;
				}
				else // if operator
				{
					i = x-1;
					break;
				}
			}
        }
		// parse Operator
		else if(str[i]=='='&&(str[i+1]=='='||str[i+1]=='>'||str[i+1]=='<'||str[i+1]==':'))
		{
			buffer[k++] = str[i];
			buffer[k++] = str[i+1];
			i ++;
		}
		else if((str[i]=='=')||(str[i]==':')||(str[i]=='+')||(str[i]=='-')||(str[i]=='*')||(str[i]=='/')||(str[i]=='%')||(str[i]=='.')||(str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]==']')||(str[i]=='{')||(str[i]=='}')||(str[i]=='(')||(str[i]==')'))
		{
			buffer[k++] = str[i];
		}
		// parse Comment
		else if (str[i] == '$' && str[i+1] == '$')
		{
			buffer[k++] = str[i];
			buffer[k++] = str[i+1];
			i ++;
		}
		else
        {
            buffer[k++] = str[i];
        }


		if(strlen(buffer) > 0)
		{
            checkToken(buffer);
			memset(buffer,0,strlen(buffer));
			k=0;
		}
	}
}

