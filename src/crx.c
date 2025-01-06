#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {
	TOKEN_PLUS,
	TOKEN_ASTERISK,
	TOKEN_WILDCARD,
	TOKEN_LITERAL,
	TOKEN_ALTERNATOR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACK,
	TOKEN_RBRACK,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_COMMA,
	TOKEN_EOF,
	TOKEN_ESCAPE_SEQUENCE,
	TOKEN_CARET,
} TokenType;

typedef struct {
	TokenType type;
	char* value;
} Token;

int main() {
	const char *regex = "\\/\\*\\*.*?[^\\/]\\*\\*\\/"; // regex for matching multiline comments in /** **/ form
	const char *p = regex;
	int length = strlen(regex);
	Token* tokens = malloc(sizeof(Token) * length); //cannot have more tokens than regex characters (i think?)
	Token *tok = tokens;
	while (*p) {
		if (isalpha(*p)) {
			printf("TOKEN_LITERAL : %c\n", *p);
			tok->type = TOKEN_LITERAL;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '*') {
			printf("TOKEN_ASTERISK : %c\n", *p);
			tok->type = TOKEN_ASTERISK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '+') {
			printf("TOKEN_PLUS : %c\n", *p);
			tok->type = TOKEN_PLUS;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '^') {
			printf("TOKEN_CARET : %c\n", *p);
			tok->type = TOKEN_CARET;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '.') {
			printf("TOKEN_WILDCARD : %c\n", *p);
			tok->type = TOKEN_WILDCARD;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '|') {
			printf("TOKEN_ALTERNATOR : %c\n", *p);
			tok->type = TOKEN_ALTERNATOR;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '(') {
			printf("TOKEN_LPAREN : %c\n", *p);
			tok->type = TOKEN_LPAREN;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ')') {
			printf("TOKEN_RPAREN : %c\n", *p);
			tok->type = TOKEN_RPAREN;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '[') {
			printf("TOKEN_LBRACK : %c\n", *p);
			tok->type = TOKEN_LBRACK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ']') {
			printf("TOKEN_RBRACK : %c\n", *p);
			tok->type = TOKEN_RBRACK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '{') {
			printf("TOKEN_LBRACE : %c\n", *p);
			tok->type = TOKEN_LBRACE;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '}') {
			printf("TOKEN_RBRACE : %c\n", *p);
			tok->type = TOKEN_RBRACE;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ',') {
			printf("TOKEN_COMMA : %c\n", *p);
			tok->type = TOKEN_COMMA;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '\\') {
			printf("TOKEN_ESCAPE_SEQUENCE : %c%c\n", *p, *(p+1));
			tok->type = TOKEN_ESCAPE_SEQUENCE;
			tok->value = malloc(3);
			tok->value[0] = *p;
			p++;
			tok->value[1] = *p;
			tok->value[2] = '\0';
		}
		p++;
	}
}
