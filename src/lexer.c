#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct lexer_T *lexer_new()
{
	struct lexer_T *lex = malloc(sizeof(struct lexer_T));

	lex->lpos = 0;
	lex->cpos = -1;

	return lex;
}

int lexer_advance(struct lexer_T *lexer) {
	if(++lexer->cpos < (int)strlen(lexer->line)) {
		lexer->ch = lexer->line[lexer->cpos];
		return 1;
	}

	return 0;
}

void lexer_run(struct lexer_T *lexer, const char *const filepath)
{
	FILE *fp;
	fopen_s(&fp, filepath, "r");

	if(!fp) {
		fprintf(stderr, "error: no such file or directory `%s`\n", filepath);
		exit(1);
	}

	const size_t line_len = 512;
	char *line = malloc(line_len);

	while(fgets(line, line_len, fp)) {
		lexer->line = line;

		while(lexer_advance(lexer)) {
			// handle lexer
		}

		lexer->cpos = -1;
	}
}
