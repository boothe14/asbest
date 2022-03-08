#ifndef LEXER_H
#define LEXER_H

struct lexer_T {
	char ch;
	int cpos; // char pos of current line
	int lpos; // line pos
	char *line;
	char *filepath;
};

enum tokentype_T {
	TOKEN_OPERATOR,
	TOKEN_CONSTANT,
	TOKEN_IDENTIFIER,
	TOKEN_KEYWORD,
	TOKEN_SYMBOL
};

struct token_T {
	int type;
	char *value;
};

static const char *const KEYWORDS[] = {
	"write",
	"int",
	"float"
};

static struct lexer_T lexer = {0};

void lexer_init();
void lexer_run(const char *const filepath);

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#endif
