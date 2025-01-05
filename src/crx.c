#include <stdio.h>
#include <string.h>

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
	TOKEN_ESCAPE,
} TokenType;

int main() {
	const char *regex = "\\/\\*\\*.*?[^\\/]\\*\\*\\/"; // regex for matching multiline comments in /** **/ form
}
