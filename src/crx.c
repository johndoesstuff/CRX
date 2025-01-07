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
	TOKEN_QUESTION,
} TokenType;

typedef struct {
	TokenType type;
	char* value;
} Token;

typedef enum {
	NODE_LITERAL,		//single character and escape
	NODE_ALTERNATION,	//(a|b)
	NODE_SUBEXPRESSION,
	NODE_REPETITION,	//(*, +, ?, {n,m})
	NODE_GROUP,		//()
	NODE_CHARACTER_CLASS,	//[a-z] [abc] etc..
	NODE_WILDCARD,		//.
	NODE_NEGATED_CLASS,	//[^abc]
	NODE_REGEX,		//entire expression
	NODE_EXPRESSION,
} NodeType;

typedef struct ASTNode {
	NodeType type;
	char *value;
	int minRepetitions;
	int maxRepetitions;
	struct ASTNode *left;
	struct ASTNode *right;
} ASTNode;

Token* Tokenize(const char *regex, int *tokCount) {
	const char *p = regex;
	int length = strlen(regex);
	*tokCount = 0;
	Token* tokens = malloc(sizeof(Token) * length); //cannot have more tokens than regex characters (i think?)
	Token *tok = tokens;
	while (*p) {
		if (*p == '*') {
			//printf("TOKEN_ASTERISK : %c\n", *p);
			tok->type = TOKEN_ASTERISK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '+') {
			//printf("TOKEN_PLUS : %c\n", *p);
			tok->type = TOKEN_PLUS;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '^') {
			//printf("TOKEN_CARET : %c\n", *p);
			tok->type = TOKEN_CARET;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '.') {
			//printf("TOKEN_WILDCARD : %c\n", *p);
			tok->type = TOKEN_WILDCARD;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '?') {
			//printf("TOKEN_QUESTION : %c\n", *p);
			tok->type = TOKEN_QUESTION;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '|') {
			//printf("TOKEN_ALTERNATOR : %c\n", *p);
			tok->type = TOKEN_ALTERNATOR;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '(') {
			//printf("TOKEN_LPAREN : %c\n", *p);
			tok->type = TOKEN_LPAREN;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ')') {
			//printf("TOKEN_RPAREN : %c\n", *p);
			tok->type = TOKEN_RPAREN;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '[') {
			//printf("TOKEN_LBRACK : %c\n", *p);
			tok->type = TOKEN_LBRACK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ']') {
			//printf("TOKEN_RBRACK : %c\n", *p);
			tok->type = TOKEN_RBRACK;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '{') {
			//printf("TOKEN_LBRACE : %c\n", *p);
			tok->type = TOKEN_LBRACE;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '}') {
			//printf("TOKEN_RBRACE : %c\n", *p);
			tok->type = TOKEN_RBRACE;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == ',') {
			//printf("TOKEN_COMMA : %c\n", *p);
			tok->type = TOKEN_COMMA;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		} else if (*p == '\\') {
			//printf("TOKEN_ESCAPE_SEQUENCE : %c%c\n", *p, *(p+1));
			tok->type = TOKEN_ESCAPE_SEQUENCE;
			tok->value = malloc(3);
			tok->value[0] = *p;
			p++;
			tok->value[1] = *p;
			tok->value[2] = '\0';
		} else if (*p != '/') {
			//printf("TOKEN_LITERAL : %c\n", *p);
			tok->type = TOKEN_LITERAL;
			tok->value = malloc(2);
			tok->value[0] = *p;
			tok->value[1] = '\0';
		}
		p++;
		tok++;
		(*tokCount)++;
	}
	tokens = realloc(tokens, *tokCount * sizeof(Token));
	return tokens;
}

Token Peek(const Token* tokens, int *position) {
	return tokens[*position];
}

Token Consume(TokenType type, const Token* tokens, int *position) {
	if (Peek(tokens, position)->type == type) {
		return tokens[(*position)++];
	} else {
		printf("Tried to consume token of type %d but found %d", type, Peek(tokens, position)->type);
		exit(1);
	}
}

ASTNode *Parse(const Token* tokens, int tokenCount) {
	const Token* token = tokens;
	ASTNode *regex = malloc(sizeof(ASTNode));
	int position = 0;
	regex->type = NODE_REGEX;
	regex->left = ConsumeExpression(tokens, tokenCount, &position);
	return regex;
}

ASTNode *ConsumeExpression(const Token* tokens, int tokenCount, int *position) {
	ASTNode *node = malloc(sizeof(ASTNode));
	node->type = NODE_EXPRESSION;
	node->left = ConsumeSubExpression(tokens, tokenCount, position);
	if (Peek(tokens, position).type == TOKEN_ALTERNATOR) {
		Consume(TOKEN_ALTERNATOR, tokens, position);
		node->right = ConsumeExpression(tokens, tokenCount, position);
	}
	return node;
}

ASTNode *ConsumeSubExpression(const Token* tokens, int tokenCount, int *position) {
	ASTNode *node = malloc(sizeof(ASTNode));
	node->type = NODE_SUBEXPRESSION;
	node->left = ConsumeSubExpressionItem(tokens, tokenCount, position);
	if (ConsumeSubExpression(tokens, tokenCount, position)) {
		node->right = ConsumeSubExpression(tokens, tokenCount, position);
	}
	return node;
}

int main() {
	//const char *regex = "\\/\\*\\*.*?[^\\/]\\*\\*\\/"; // regex for matching multiline comments in /** **/ form
	const char *regex = "\\(\\d{3}\\) \\d{3}-\\d{4}"; // regex for matching phone numbers
	int tokenCount = 0;
	const Token* tokens = Tokenize(regex, &tokenCount);
	for (int i = 0; i < tokenCount; i++) {
		printf("Token %d; %s\n", i, tokens[i].value);
	}
	ASTNode *parsedRegex = Parse(tokens, tokenCount);
}
