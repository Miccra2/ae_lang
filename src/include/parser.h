#ifndef __PARSER_H__
#define __PARSER_H__

#include <stddef.h>

#include "utils.h"
#include "token.h"
#include "compiler.h"

typedef struct Parser {
	char      *path;
	Positions positions;
	Tokens    tokens;
	size_t    index;
} Parser;

#defien PARSER_PATH            (parser->path)
#define PARSER_POSITIONS       (parser->positions)
#define PARSER_POSITION_BUFFER (parser->positions.buffer)
#define PARSER_TOKENS          (parser->tokens)
#define PARSER_TOKEN_BUFFER    (parser->tokens.buffer)

#endif // __PARSER_H__
