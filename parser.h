//This header file declares functions defined in parser.c for parsing contents of input file.

#pragma once
#include <stdbool.h>
#include "testscanner.h"

struct TreeNode;

typedef struct Array {
	struct TreeNode* value;
	int size, msize;
} Array;

typedef struct TreeNode {
	char* label;
	tokenID id;
	Array children;
} TreeNode;

extern int lineCount;
void push_back(Array *array, TreeNode* val);
TreeNode* newNode(char* value, tokenID type);
bool isKWToken(token, char *s);
bool isOPToken(token, char *s);
void getNextToken();
TreeNode* parse();
TreeNode* program();
TreeNode* block();
TreeNode* vars();
TreeNode* expr();
TreeNode* N();
TreeNode* A();
TreeNode* M();
TreeNode* R();
TreeNode* stats();
TreeNode* mStat();
TreeNode* stat();
TreeNode* in();
TreeNode* out();
TreeNode* IF();
TreeNode* loop();
TreeNode* assign();
TreeNode* RO();
TreeNode* label();
TreeNode* GOTO();
void printTreePreOrder(TreeNode*, int);
void error(char*, char*);

