#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int lexer_advance() {
	if(++lexer.cpos < (int)strlen(lexer.line)) {
		lexer.ch = lexer.line[lexer.cpos];
		return 1;
	}

	return 0;
}

const char *toktype_as_str(int type) {
	switch(type) {
		case TOKEN_OPERATOR: return "OPERATOR";
		case TOKEN_CONSTANT: return "CONSTANT";
		case TOKEN_IDENTIFIER: return "IDENTIFIER";
		case TOKEN_KEYWORD: return "KEYWORD";
		case TOKEN_SYMBOL: return "SYMBOL";
		default: {
			fprintf(stderr, "%s: exhaustive handling of enums encountered in: toktype_as_str() \n", lexer.filepath);
			exit(-1);
		}
	}
}

const char *tok_as_str(const struct token_T tok) {
	char *fmt_str = malloc(2048);
	snprintf(fmt_str, 2048, "(%s, %s)", toktype_as_str(tok.type), tok.value);
	return fmt_str;
}

void lexer_init() {
	lexer.cpos = -1;
}

bool isspecial(char c) {
	switch(c) {
		case '~': case '|':
		case '!': case '\\':
		case '#': case '`':
		case '$': case '-':
		case '%': case '=':
		case '^': case '<':
		case '&': case '>':
		case '*': case '?':
		case '(': case '{':
		case ')': case '}':
		case '_': case '[':
		case '+': case ']':
		case ',': case ':':
		case '.': case '"':
		case '/':

		return 1;

		default: return 0;
	}
}

const char *lexer_make_number() {

	char *str = malloc(512);
	str[0] = 0;

	do {
		if(isdigit(lexer.ch) || lexer.ch == '.') {

			snprintf(str, 512, "%s%c", str, lexer.ch);
		} else if(!isspace(lexer.ch)) {
			fprintf(stderr, "%s:%d:%d: illegal character in numeral literal `%c` \n", lexer.filepath, lexer.lpos, lexer.cpos, lexer.ch);
			exit(1);
		}
	} while(lexer_advance() && !isspace(lexer.ch) && !isspecial(lexer.ch));
	lexer.cpos -= 1;

	return str;
}

bool word_in_keywords(const char *str) {
	for(size_t i = 0; i < ARRAY_SIZE(KEYWORDS); ++i) {
		if(!strcmp(str, KEYWORDS[i])) return 1;
	}

	return 0;
}

const char *lexer_make_word() {
	char *str = malloc(2048);
	str[0] = 0;

	do {
		snprintf(str, 2048, "%s%c", str, lexer.ch);
	} while(lexer_advance() && !isspace(lexer.ch) && !isspecial(lexer.ch));
	lexer.cpos -= 1;

	return str;
}

const char *lexer_make_string() {
	char *str = malloc(2048);
	str[0] = 0;

	int att = 0;

	lexer_advance();

	int cpos = lexer.cpos;
	int lpos = lexer.lpos;

	for(;lexer.ch != '"'; lexer_advance()) {
		att += 1;
		snprintf(str, 2048, "%s%c", str, lexer.ch);

		if(att > 2048) {
			fprintf(stderr, "%s:%d:%d: unclosed string literal \n", lexer.filepath, lpos, cpos);
			exit(1);
		}
	}

	return str;
}

void lexer_run(const char *const filepath)
{
	FILE *fp;
	fopen_s(&fp, filepath, "r");
	lexer.filepath = _strdup(filepath);

	if(!fp) {
		fprintf(stderr, "error: no such file or directory `%s`\n", filepath);
		exit(1);
	}

	const size_t line_len = 512;
	char *line = malloc(line_len);

	const size_t tokens_sizeof = sizeof(struct token_T)*200;
	struct token_T *tokens = malloc(tokens_sizeof);
	size_t tokens_len = 0;

	int skip = 0;

	while(fgets(line, line_len, fp)) {
		lexer.line = line;

		char *context = malloc(512);
		strtok_s(lexer.line, "//", &context);

		if(strlen(lexer.line) >= 2) {
			if(lexer.line[0] == '/' && lexer.line[1] == '/') {
				skip = 1;
				lexer.lpos += 1;
			} else {
				skip = 0;
			}
		}

		while(lexer_advance() && !skip) {

			switch(lexer.ch) {
				case '+': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_OPERATOR, .value="+"};
				}
				break;

				case '-': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_OPERATOR, .value="-"};
				}
				break;

				case '*': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_OPERATOR, .value="*"};
				}
				break;

				case '/': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_OPERATOR, .value="/"};
				}
				break;

				case '(': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_SYMBOL, .value="("};
				}
				break;

				case ')': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_SYMBOL, .value=")"};
				}
				break;

				case '{': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_SYMBOL, .value="{"};
				}
				break;

				case '}': {
					tokens[tokens_len++] = (struct token_T){.type=TOKEN_SYMBOL, .value="}"};
				}
				break;

				case '"': {
					char *str = malloc(2048);
					str[0] = 0;

					memcpy(str, lexer_make_string(), 2048);

					tokens[tokens_len++] = (struct token_T){.type=TOKEN_CONSTANT, .value=str};
				}
				break;

				default: {
					if(isdigit(lexer.ch)) {
						char *num = malloc(512);

						strcpy_s(num, 512, lexer_make_number());

						tokens[tokens_len++] = (struct token_T){.type=TOKEN_CONSTANT, .value=num};
					} else if(!isspace(lexer.ch)) {
						char *word = malloc(2048);
						memcpy(word, lexer_make_word(), 2048);

						if(!word_in_keywords(word)) {
							tokens[tokens_len++] = (struct token_T){.type=TOKEN_IDENTIFIER, .value=word};
						} else {
							tokens[tokens_len++] = (struct token_T){.type=TOKEN_KEYWORD, .value=word};
						}
					}
				}
				break;
			}
		}

		lexer.cpos = -1;
	}


	for(size_t i = 0; i < tokens_len; ++i) {
		printf("%s\n", tok_as_str(tokens[i]));
	}
}
