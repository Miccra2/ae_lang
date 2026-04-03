#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "utils.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"

void token_info(Lexer *lexer) {
	size_t len = LEX_TOKENS.length;
	printf("toekns: %zu\n", len);
	for (size_t i = 0; i < LEX_TOKENS.length; i++) {
		Position pos = LEX_POSITION_BUFFER[i];
		TokenKind token  = LEX_TOKEN_BUFFER[i];
		printf("Info: %zu:%zu:%s: Token[%zu](kind: %s(%zu), Position(begin: %zu, end: %zu, offset: %zu, line: %zu))\n", 
			pos.line, position_column(pos), LEX_PATH, i, token_kind[(size_t)token], (size_t)token, 
			(size_t)(pos.begin - LEX_TEXT + 1), (size_t)(pos.end - LEX_TEXT + 1), (size_t)(pos.offset - LEX_TEXT + 1), pos.line);
	}
}

void expression_info(Parser *parser, Expression expr, size_t depth) {
	depth++;
	Position begin_pos = PARSER_POSITION_BUFFER[expr.begin];
	Position end_pos = PARSER_POSITION_BUFFER[expr.end];
	printf("Expression(\n");
	printf("%*skind:  %s(%zu),\n", (int)(depth * 4), "", expr_kind[(size_t)expr.kind], (size_t)expr.kind);
	printf("%*sbegin: Position(begin: %zu, end: %zu, offset: %zu, line: %zu),\n", (int)(depth * 4), "", 
		(size_t)(begin_pos.begin - PARSER_TEXT + 1), (size_t)(begin_pos.end - PARSER_TEXT + 1), (size_t)(begin_pos.offset - PARSER_TEXT + 1), begin_pos.line);
	printf("%*send:   Position(begin: %zu, end: %zu, offset: %zu, line: %zu),\n", (int)(depth * 4), "", 
		(size_t)(end_pos.begin - PARSER_TEXT + 1), (size_t)(end_pos.end - PARSER_TEXT + 1), (size_t)(end_pos.offset - PARSER_TEXT + 1), end_pos.line);
	switch (expr.kind) {
	case EXPR_VALUE: printf("%*svalue: %zu,\n", (int)(depth * 4), "", expr.value); break;
	case EXPR_IDENTIFIER:
		Position pos = PARSER_POSITION_BUFFER[expr.begin];
		size_t len = position_length(pos);
		char *buf = malloc(len + 1); 
		strncpy(buf, pos.begin, len);
		buf[len] = 0;
		printf("%*sidentifier: %s\n", (int)(depth * 4), "", buf); 
		break;
	case EXPR_INFIX:
		printf("%*sInfixExpr(\n", (int)(depth * 4), "");
		printf("%*skind: %s\n", (int)(depth * 4), "", token_kind[(size_t)expr.infix.kind]);
		printf("%*slhs:  ", (int)(++depth * 4), "");
		expression_info(parser, *expr.infix.lhs, depth);
		printf("%*srhs:  ", (int)(depth * 4), "");
		expression_info(parser, *expr.infix.rhs, depth--);
		printf("%*s),\n", (int)(depth * 4), "");
		break;
	default: break;
	}
	printf("%*s),\n", (int)(--depth * 4), "");
}

void expr_info(Parser *parser) {
	Position pos = PARSER_POSITION_BUFFER[parser->expr->begin];
	printf("Info: %zu:%zu:%s: ", pos.line, position_column(pos), PARSER_PATH);
	expression_info(parser, *parser->expr, 0);
}

int main(void) {
	char *path = "test.aec";
	Lexer lexer = new_lexer(path);
	tokenise(&lexer);
	Parser parser = new_parser(&lexer);
	parse(&parser);
	expr_info(&parser);
	del_expression(&parser.expr);
	del_parser(&parser);
	del_lexer(&lexer);
	return 0;
}
