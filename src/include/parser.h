#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "ast.h"
#include "utils.h"
#include "token.h"
#include "lexer.h"

typedef struct Parser {
	char       *path;
	char       *text;
	Positions  positions;
	Tokens     tokens;
	size_t     index;
	Expression *expr;
} Parser;

#define PARSER_PATH            (parser->path)
#define PARSER_TEXT            (parser->text)
#define PARSER_POSITIONS       (parser->positions)
#define PARSER_POSITION_BUFFER (parser->positions.buffer)
#define PARSER_TOKENS          (parser->tokens)
#define PARSER_TOKEN_BUFFER    (parser->tokens.buffer)
#define PARSER_INDEX           (parser->index)
#define PARSER_CURR_POSITION   (parser->positions.buffer[parser->index])
#define PARSER_CURR_TOKEN      (parser->tokens.buffer[parser->index])
#define PARSER_LINE            (parser->positions.buffer[parser->index].line)
#define PARSER_COLUMN          (position_column(parser->positions.buffer[parser->index]))
#define PARSER_EXPR            (parser->expr)

Parser new_parser(Lexer *lexer);
void del_parser(Parser *parser);
bool expect_tokens(Parser *parser, size_t offset, TokenKind *kind, size_t length);
Expression parse_value_expr(Parser *parser);
Expression parse_infix_expr(Parser *parser, size_t index, TokenKind kind, Expression lhs);
Expression parse_expression(Parser *parser, size_t priority);
void parse(Parser *parser);

#endif // __PARSER_H__
