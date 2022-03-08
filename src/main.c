#include <stdio.h>
#include "lexer.h"

int main(void) {
	struct lexer_T *lexer = lexer_new();

	lexer_run(lexer, "Makefile");

	return 0;
}
