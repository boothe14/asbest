#ifndef LEXER_H
#define LEXER_H

struct lexer_T {
	char ch;
	int cpos; // char pos of current line
	int lpos; // line pos
	char *line;
};

struct lexer_T *lexer_new();
void lexer_run(struct lexer_T *lexer, const char *const filepath);

#endif
