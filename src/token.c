#include "token.h"

char *token_kind[] = {
	"UNDEFINED",
	"ERROR",
	"EOF",
	"IDENTIFIER",
	"INTEGER_LITERAL",
	"EQUALS",
	"PLUS",
	"MINUS",
	"ASTERISK",
	"SLASH",
	"DOT",
	"COLON",
	"COMMA",
	"SEMI_COLON",
	"AT",
	"HASHTAG",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"OPEN_PARENTHESE",
	"CLOSE_PARENTHESE",
	"OPEN_BRACE",
	"CLOSE_BRACE",
	"OPEN_BRACKET",
	"CLOSE_BRACKET",
};

Position new_position(char *begin, char *end, char *offset, size_t line) {
	return (Position){.begin=begin, .end=end, .offset=offset, .line=line};
}

size_t position_length(Position position) {
	return position.end - position.begin;
}

size_t position_column(Position position) {
	return position.begin - position.offset + 1;
}

Token new_token(TokenKind kind, Position position) {
	return (Token){.kind=kind, .position=position};
}
