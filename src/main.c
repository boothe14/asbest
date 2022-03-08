#include <stdio.h>
#include "lexer.h"

int main(void) {
	lexer_init();
	lexer_run("examples/hello.lang");

	return 0;
}
