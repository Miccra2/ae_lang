#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stddef.h>

typedef struct Position {
	char   *begin;		// begin of token
	char   *end;		// end of token
	char   *offset;		// begin for line of token
	size_t line;		// linenumber of token
} Position;

typedef struct Positions {
	size_t   capacity;
	size_t   length;
	Position *buffer;
} Positions;

typedef enum TokenKind {
	TOKEN_UNDEFINED,		// internal
	TOKEN_ERROR,			// internal
	TOKEN_EOF,				// internal
	TOKEN_IDENTIFIER,		// "[_A-Za-z][_A-Za-z0-9]*"
	TOKEN_INTEGER_LITERAL,	// "[0-9]"
	TOKEN_EQUAL,			// "="
	TOKEN_PLUS,				// "+"
	TOKEN_MINUS,			// "-"
	TOKEN_ASTERISK,			// "*"
	TOKEN_SLASH,			// "/"
	TOKEN_DOT,				// "."
	TOKEN_COLON,			// ":"
	TOKEN_COMMA,            // ","
	TOKEN_SEMI_COLON,		// ";"
	TOKEN_AT,               // "@"
	TOKEN_HASHTAG,          // "#"
	TOKEN_SINGLE_QUOTE,     // "'"
	TOKEN_DOUBLE_QUOTE,     // "\""
	TOKEN_OPEN_PARENTHESE,	// "("
	TOKEN_CLOSE_PARENTHESE,	// ")"
	TOKEN_OPEN_BRACE,		// "{"
	TOKEN_CLOSE_BRACE,		// "}"
	TOKEN_OPEN_BRACKET,		// "["
	TOKEN_CLOSE_BRACKET,	// "]"
} TokenKind;

typedef struct Token {
	TokenKind kind;
	Position  position;
} Token;

typedef struct Tokens {
	size_t    capacity;
	size_t    length;
	TokenKind *buffer;
} Tokens;

extern char *token_kind[];

Position new_position(char *begin, char *end, char *offset, size_t line);
size_t position_length(Position position);
size_t position_column(Position position);
Token new_token(TokenKind kind, Position position);
size_t token_priority(TokenKind kind);

#endif // __TOKEN_H__
