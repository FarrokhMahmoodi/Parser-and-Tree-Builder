#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

TreeNode* newNode(char* value, tokenID id) {
	TreeNode* newnode = (TreeNode*)malloc(sizeof(TreeNode));
	int len = strlen(value);
	newnode->label = malloc(len+1);
	strcpy(newnode->label, value);
	newnode->id = id;
	newnode->children.size = 0;
	newnode->children.msize = 0;
	return newnode;
}

bool isKWToken(token tok, char *s) {
	return tok.id == KW_tk && strcmp(tok.str, s) == 0;
}

bool isOPToken(token tok, char *s) {
	return tok.id == OPER_tk && strcmp(tok.str, s) == 0;
}

token curToken;
int lineCount = 0;
char buffer[100];

bool Isalpha(int c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool Isspace(int c) {
	return c == ' ' || c == '\n' || c == '\t' || c == 10 || c == 13;
}

bool Isdigit(int c) {
	return c >= '0' && c <= '9';
}

void getNextToken() {
	char c;
	while (c = getchar(), Isspace(c)) if (c == '\n') lineCount++;
	int i = 0;
	for (; !Isspace(c) && c != -1; buffer[i++] = c, c = getchar());
	if (c == '\n') lineCount++;
	buffer[i] = 0;
	curToken = checkToken(buffer);
	if (curToken.id == COMMENT_tk) getNextToken();
}

void push_back(Array *array, TreeNode* val) {
	if (array->msize <= array->size + 1) {
		int tsz = (array->msize + 1) * 2;
		TreeNode* tmp = (TreeNode*)malloc(sizeof(TreeNode)*tsz);
		int i;
		for (i = 0; i < array->size; i++) {
			tmp[i] = array->value[i];
		}
		array->value = tmp;
		array->msize = tsz;
	}
	array->value[array->size ++] = *val;
}

TreeNode* parse() {
	getNextToken();
	return program();
}

TreeNode* program() {
	TreeNode* _vars = vars();
	if (_vars == NULL) return NULL;
	if (!isKWToken(curToken, "main")) return error("main", curToken.str), NULL;
	TreeNode* _main = newNode("main", KW_tk);
	getNextToken();
	TreeNode* _block = block();
	if (_block == NULL) return NULL;
	TreeNode* _program = newNode("program", NO_tk);
	push_back(&_program->children, _vars);
	push_back(&_program->children, _main);
	push_back(&_program->children, _block);
	return _program;
}

TreeNode* block() {
	if (!isKWToken(curToken, "begin")) return error("begin", curToken.str), NULL;
	TreeNode* _begin = newNode("begin", KW_tk);
	getNextToken();
	TreeNode* _vars = vars();
	if (_vars == NULL) return NULL;
	TreeNode* _stats = stats();
	if (_stats == NULL) return NULL;
	if (!isKWToken(curToken, "end")) return error("end", curToken.str), NULL;
	TreeNode* _end = newNode("end", KW_tk);
	TreeNode* _block = newNode("block", NO_tk);
	push_back(&_block->children, _begin);
	push_back(&_block->children, _vars);
	push_back(&_block->children, _stats);
	push_back(&_block->children, _end);
	getNextToken();
	return _block;
}

TreeNode* vars() {
	if (!isKWToken(curToken, "data")) {
		TreeNode* _vars = newNode("vars", NO_tk);
		return _vars;
	}
	TreeNode* _data = newNode("data", KW_tk);
	getNextToken();
	if (curToken.id != IDENT_tk) return error("Identifier", curToken.str), NULL;
	TreeNode* _iden = newNode(curToken.str, IDENT_tk);
	getNextToken();
	if (!isOPToken(curToken, ":=")) return error(":=", curToken.str), NULL;
	TreeNode* _op = newNode(":=", OPER_tk);
	getNextToken();
	if (curToken.id != NUM_tk) return error("Number", curToken.str), NULL;
	TreeNode* _num = newNode(curToken.str, NUM_tk);
	getNextToken();
	if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
	TreeNode* _semi = newNode(";", OPER_tk);
	getNextToken();
	TreeNode* _child_vars = vars();
	if (_child_vars == NULL) return NULL;
	TreeNode* _vars = newNode("vars", NO_tk);
	push_back(&_vars->children, _data);
	push_back(&_vars->children, _iden);
	push_back(&_vars->children, _op);
	push_back(&_vars->children, _num);
	push_back(&_vars->children, _semi);
	push_back(&_vars->children, _child_vars);
	return _vars;
}

TreeNode* expr() {
	TreeNode* _N = N();
	if (_N == NULL) return NULL;
	if (isOPToken(curToken, "-")) {
		TreeNode* _minus = newNode("-", OPER_tk);
		getNextToken();
		TreeNode* _child_expr = expr();
		if (_child_expr == NULL) return NULL;
		TreeNode* _expr = newNode("expr", NO_tk);
		push_back(&_expr->children, _N);
		push_back(&_expr->children, _minus);
		push_back(&_expr->children, _child_expr);
		return _expr;
	}
	TreeNode* _expr = newNode("expr", NO_tk);
	push_back(&_expr->children, _N);
	return _expr;
}

TreeNode* N() {
	TreeNode* _A = A();
	if (_A == NULL) return NULL;
	if (isOPToken(curToken, "/") || isOPToken(curToken, "*")) {
		TreeNode* _op = newNode(curToken.str, OPER_tk);
		getNextToken();
		TreeNode* _child_N = N();
		if (_child_N == NULL) return NULL;
		TreeNode* _N = newNode("N", NO_tk);
		push_back(&_N->children, _A);
		push_back(&_N->children, _op);
		push_back(&_N->children, _child_N);
		return _N;
	}
	TreeNode* _N = newNode("N", NO_tk);
	push_back(&_N->children, _A);
	return _N;
}

TreeNode* A() {
	TreeNode* _M = M();
	if (_M == NULL) return NULL;
	if (isOPToken(curToken, "+")) {
		TreeNode* _op = newNode(curToken.str, OPER_tk);
		getNextToken();
		TreeNode* _child_A = A();
		if (_child_A == NULL) return NULL;
		TreeNode* _A = newNode("N", NO_tk);
		push_back(&_A->children, _M);
		push_back(&_A->children, _op);
		push_back(&_A->children, _child_A);
		return _A;
	}
	TreeNode* _A = newNode("N", NO_tk);
	push_back(&_A->children, _M);
	return _A;
}

TreeNode* M() {
	if (isOPToken(curToken, "*")) {
		TreeNode* _star = newNode("*", OPER_tk);
		getNextToken();
		TreeNode* _child_M = M();
		if (_child_M == NULL) return NULL;
		TreeNode* _M = newNode("M", NO_tk);
		push_back(&_M->children, _star);
		push_back(&_M->children, _child_M);
		return _M;
	}
	TreeNode* _R = R();
	if (_R == NULL) return NULL;
	TreeNode* _M = newNode("M", NO_tk);
	push_back(&_M->children, _R);
	return _M;
}

TreeNode* R() {
	if (isOPToken(curToken, "(")) {
		TreeNode* _lpar = newNode("(", OPER_tk);
		getNextToken();
		TreeNode* _expr = expr();
		if (_expr == NULL) return NULL;
		if (!isOPToken(curToken, ")")) return error(")", curToken.str), NULL;
		TreeNode* _rpar = newNode(")", OPER_tk);
		TreeNode* _R = newNode("R", NO_tk);
		push_back(&_R->children, _lpar);
		push_back(&_R->children, _expr);
		push_back(&_R->children, _rpar);
		getNextToken();
		return _R;
	}
	if (curToken.id == IDENT_tk) {
		TreeNode* _iden = newNode(curToken.str, IDENT_tk);
		TreeNode* _R = newNode("R", NO_tk);
		push_back(&_R->children, _iden);
		getNextToken();
		return _R;
	}
	if (curToken.id == NUM_tk) {
		TreeNode* _num = newNode(curToken.str, NUM_tk);
		TreeNode* _R = newNode("R", NO_tk);
		push_back(&_R->children, _num);
		getNextToken();
		return _R;
	}
	return error("( or Identifier or Number", curToken.str), NULL;
}

TreeNode* stats() {
	TreeNode* _stat = stat();
	if (_stat == NULL) return NULL;
	TreeNode* _mStat = mStat();
	if (_mStat == NULL) return NULL;
	TreeNode* _stats = newNode("stats", NO_tk);
	push_back(&_stats->children, _stat);
	push_back(&_stats->children, _mStat);
	return _stats;
}

TreeNode* mStat() {
	if (isKWToken(curToken, "end")) {
		TreeNode* _mStat = newNode("mStat", NO_tk);
		return _mStat;
	}
	TreeNode* _stat = stat();
	if (_stat == NULL) return NULL;
	TreeNode* _child_mStat = mStat();
	if (_child_mStat == NULL) return NULL;
	TreeNode* _mStat = newNode("mStat", NO_tk);
	push_back(&_mStat->children, _stat);
	push_back(&_mStat->children, _child_mStat);
	return _mStat;
}

TreeNode* stat() {
	if (isKWToken(curToken, "getter")) {
		TreeNode* _in = in();
		if (_in == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _in);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "outter")) {
		TreeNode* _out = out();
		if (_out == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _out);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "begin")) {
		TreeNode* _block = block();
		if (_block == NULL) return NULL;
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _block);
		return _stat;
	}
	if (isKWToken(curToken, "if")) {
		TreeNode* _IF = IF();
		if (_IF == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _IF);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "loop")) {
		TreeNode* _loop = loop();
		if (_loop == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _loop);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "assign")) {
		TreeNode* _assign = assign();
		if (_assign == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _assign);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "proc")) {
		TreeNode* _GOTO = GOTO();
		if (_GOTO == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _GOTO);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	if (isKWToken(curToken, "void")) {
		TreeNode* _label = label();
		if (_label == NULL) return NULL;
		if (!isOPToken(curToken, ";")) return error(";", curToken.str), NULL;
		TreeNode* _semi = newNode(";", OPER_tk);
		TreeNode* _stat = newNode("stat", NO_tk);
		push_back(&_stat->children, _label);
		push_back(&_stat->children, _semi);
		getNextToken();
		return _stat;
	}
	return error("getter or outter or if or loop or proc or void", curToken.str), NULL;
}

TreeNode* in() {
	if (!isKWToken(curToken, "getter")) return error("getter", curToken.str), NULL;
	TreeNode* _getter = newNode("getter", KW_tk);
	getNextToken();
	if (curToken.id != IDENT_tk) return error("Identifier", curToken.str), NULL;
	TreeNode* _iden = newNode(curToken.str, IDENT_tk);
	TreeNode* _in = newNode("in", NO_tk);
	push_back(&_in->children, _getter);
	push_back(&_in->children, _iden);
	getNextToken();
	return _in;
}

TreeNode* out() {
	if (!isKWToken(curToken, "outter")) return error("outter", curToken.str), NULL;
	TreeNode* _outter = newNode("outter", KW_tk);
	getNextToken();
	TreeNode* _expr = expr();
	if (_expr == NULL) return NULL;
	TreeNode* _out = newNode("out", NO_tk);
	push_back(&_out->children, _outter);
	push_back(&_out->children, _expr);
	return _out;
}

TreeNode* IF() {
	if (!isKWToken(curToken, "if")) return error("if", curToken.str), NULL;
	TreeNode* _IF_ = newNode("if", KW_tk);
	getNextToken();
	if (!isOPToken(curToken, "[")) return error("[", curToken.str), NULL;
	TreeNode* _lpar = newNode("[", OPER_tk);
	getNextToken();
	TreeNode* _expr1 = expr();
	if (_expr1 == NULL) return NULL;
	TreeNode* _ro = RO();
	if (_ro == NULL) return NULL;
	TreeNode* _expr2 = expr();
	if (_expr2 == NULL) return NULL;
	if (!isOPToken(curToken, "]")) return error("]", curToken.str), NULL;
	TreeNode* _rpar = newNode("]", OPER_tk);
	getNextToken();
	if (!isKWToken(curToken, "then")) return error("then", curToken.str), NULL;
	TreeNode* _then = newNode("then", KW_tk);
	getNextToken();
	TreeNode* _stat = stat();
	if (_stat == NULL) return NULL;
	TreeNode* _IF = newNode("IF", NO_tk);
	push_back(&_IF->children, _IF_);
	push_back(&_IF->children, _lpar);
	push_back(&_IF->children, _expr1);
	push_back(&_IF->children, _ro);
	push_back(&_IF->children, _expr2);
	push_back(&_IF->children, _rpar);
	push_back(&_IF->children, _then);
	push_back(&_IF->children, _stat);
	return _IF;
}

TreeNode* loop() {
	if (!isKWToken(curToken, "loop")) return error("loop", curToken.str), NULL;
	TreeNode* _loop_ = newNode("loop", KW_tk);
	getNextToken();
	if (!isOPToken(curToken, "[")) return error("[", curToken.str), NULL;
	TreeNode* _lpar = newNode("[", OPER_tk);
	getNextToken();
	TreeNode* _expr1 = expr();
	if (_expr1 == NULL) return NULL;
	TreeNode* _ro = RO();
	if (_ro == NULL) return NULL;
	TreeNode* _expr2 = expr();
	if (_expr2 == NULL) return NULL;
	if (!isOPToken(curToken, "]")) return error("]", curToken.str), NULL;
	TreeNode* _rpar = newNode("]", OPER_tk);
	getNextToken();
	TreeNode* _stat = stat();
	if (_stat == NULL) return NULL;
	TreeNode* _loop = newNode("loop", NO_tk);
	push_back(&_loop->children, _loop_);
	push_back(&_loop->children, _lpar);
	push_back(&_loop->children, _expr1);
	push_back(&_loop->children, _ro);
	push_back(&_loop->children, _expr2);
	push_back(&_loop->children, _rpar);
	push_back(&_loop->children, _stat);
	return _loop;
}

TreeNode* assign() {
	if (!isKWToken(curToken, "assign")) return error("assign", curToken.str), NULL;
	TreeNode* _assign_ = newNode("assign", KW_tk);
	getNextToken();
	if (curToken.id != IDENT_tk) return NULL;
	TreeNode* _iden = newNode(curToken.str, IDENT_tk);
	getNextToken();
	if (!isOPToken(curToken, ":=")) return error(":=", curToken.str), NULL;
	TreeNode* _op = newNode(":=", OPER_tk);
	getNextToken();
	TreeNode* _expr = expr();
	if (_expr == NULL) return NULL;
	TreeNode* _assign = newNode("assign", NO_tk);
	push_back(&_assign->children, _assign_);
	push_back(&_assign->children, _iden);
	push_back(&_assign->children, _op);
	push_back(&_assign->children, _expr);
	return _assign;
}

TreeNode* RO() {
	if (isOPToken(curToken, "<") || isOPToken(curToken, "=>") || isOPToken(curToken, "=<") ||
	   isOPToken(curToken, "==") || isOPToken(curToken, "%")) {
		TreeNode* _op = newNode(curToken.str, OPER_tk);
		TreeNode* _RO = newNode("RO", NO_tk);
		push_back(&_RO->children, _op);
		getNextToken();
		return _RO;
	}
	// [ == ] (three tokens)
	return error("< or => or =< or == or %", curToken.str), NULL;
}

TreeNode* label() {
	if (!isKWToken(curToken, "void")) return error("void", curToken.str), NULL;
	TreeNode* _void = newNode("void", KW_tk);
	getNextToken();
	if (curToken.id != IDENT_tk) return error("Identifier", curToken.str), NULL;
	TreeNode* _iden = newNode(curToken.str, IDENT_tk);
	TreeNode* _label = newNode("label", NO_tk);
	push_back(&_label->children, _void);
	push_back(&_label->children, _iden);
	getNextToken();
	return _label;
}

TreeNode* GOTO() {
	if (!isKWToken(curToken, "proc")) return error("proc", curToken.str), NULL;
	TreeNode* _proc = newNode("proc", KW_tk);
	getNextToken();
	if (curToken.id != IDENT_tk) return error("Identifier", curToken.str), NULL;
	TreeNode* _iden = newNode(curToken.str, IDENT_tk);
	TreeNode* _goto = newNode("GOTO", NO_tk);
	push_back(&_goto->children, _proc);
	push_back(&_goto->children, _iden);
	getNextToken();
	return _goto;
}

void printTreePreOrder(TreeNode* node, int deep) {
	int cnt = deep, i;
	for (i = 0; i < cnt; i++) putchar(' ');
	printf("%s\n", node->label);
	for (i = 0; i < node->children.size; i++) {
		printTreePreOrder(&node->children.value[i], deep + 2);
	}
}

void error(char *a, char *b) {
	printf("At line %d, exptected tok : \"%s\", but now \"%s\"\n", lineCount, a, b); 
	exit(0);
}

