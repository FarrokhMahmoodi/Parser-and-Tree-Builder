//CS 4280
//April 12, 2021
//Farrokh Mahmoodi

//This header file declares functions defined in scanner.c for tokenizing the input from input file or printing out a token scanner error

#pragma once
#include <stdio.h>

typedef enum
{ NO_tk = 0, IDENT_tk = 1, NUM_tk = 2, KW_tk =3, OPER_tk = 4, COMMENT_tk=5, EOF_tk=6 } tokenID;

typedef struct
{
	tokenID id;
	char* str;
} token;

token checkToken(char* str);
int checkNumber(char* str);
int checkKeyword(char* str);
int checkOperator(char* str);
int checkIdentifier(char* str);
int checkEOF(char* str);
