//This .c file has main function that takes in an input file and parses it to print out a parse tree or token scanner error

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc, char *argv[]) {
	if (argc > 1) {
		char *filename = (char*)malloc(strlen(argv[1])+6);
		strcpy(filename, argv[1]);
		strcat(filename, ".fs"); //stdin
		FILE* fin = freopen(filename, "r", stdin);
		if (!fin) {
			fprintf(stderr, "Can not open the file.");
			return 0;
		}
	}

	TreeNode *root = parse();
	printTreePreOrder(root, 0);

	return 0;
}
