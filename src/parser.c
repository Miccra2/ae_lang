#include "parser.h"

Parser new_parser(Lexer *lexer) {
	return (Parser){
		.path = LEX_PATH,
		.text = LEX_TEXT,
		.positions = LEX_POSITIONS,
		.tokens = LEX_TOKENS,
		.index = 0,
		.expr = malloc(sizeof(Expression)),
	};
}

void del_parser(Parser *parser) {
	if (parser) {
		free(parser->expr);
		parser->expr = NULL;
	}
}

bool expect_tokens(Parser *parser, size_t offset, TokenKind *kinds, size_t length) {
	if (PARSER_INDEX + offset + length >= PARSER_TOKENS.length) {
		return false;
	}
	for (size_t i = 0; i < length; i++) {
		TokenKind kind = PARSER_TOKEN_BUFFER[PARSER_INDEX + offset + i];
		if (kind != kinds[i]) {
			return false;
		}
	}
	return true;
}

Expression parse_value_expr(Parser *parser) {
	size_t value = 0;
	if (PARSER_CURR_POSITION.end - PARSER_CURR_POSITION.begin > 20) {
		return new_expression(EXPR_ERROR, PARSER_INDEX, PARSER_INDEX, &value);
	}
	for (char *cur = PARSER_CURR_POSITION.begin; cur < PARSER_CURR_POSITION.end; cur++) {
		value *= 10;
		value += *cur - '0';
	}
	return new_expression(EXPR_VALUE, PARSER_INDEX, PARSER_INDEX, &value);
}

Expression parse_infix_expr(Parser *parser, size_t index, TokenKind kind, Expression lhs) {
	Expression rhs = parse_expression(parser, 0);
	InfixExpr infix = new_infix_expr(kind, lhs, rhs);
	return new_expression(EXPR_INFIX, index, PARSER_INDEX, &infix);
}

Expression parse_expression(Parser *parser, size_t priority) {
	size_t index = PARSER_INDEX;
	Expression lhs = new_expression(EXPR_UNDEFINED, -1, -1, NULL);
	if (PARSER_CURR_TOKEN == TOKEN_INTEGER_LITERAL) {
		lhs = parse_value_expr(parser); 
		PARSER_INDEX++;
	} else if (PARSER_CURR_TOKEN == TOKEN_IDENTIFIER) {
		lhs = new_expression(EXPR_IDENTIFIER, PARSER_INDEX, PARSER_INDEX, NULL);
		PARSER_INDEX++;
	}
	while (token_priority(PARSER_CURR_TOKEN) > priority) {
		if (PARSER_CURR_TOKEN == TOKEN_EQUAL) {
			PARSER_INDEX++;
			lhs = parse_infix_expr(parser, index, TOKEN_EQUAL, lhs);
		} else if (PARSER_CURR_TOKEN == TOKEN_PLUS) {
			PARSER_INDEX++;
			lhs = parse_infix_expr(parser, index, TOKEN_PLUS, lhs);
		} else if (PARSER_CURR_TOKEN == TOKEN_MINUS) {
			PARSER_INDEX++;
			lhs = parse_infix_expr(parser, index, TOKEN_MINUS, lhs);
		} else if (PARSER_CURR_TOKEN == TOKEN_ASTERISK) {
			PARSER_INDEX++;
			lhs = parse_infix_expr(parser, index, TOKEN_ASTERISK, lhs);
		} else if (PARSER_CURR_TOKEN == TOKEN_SLASH) {
			PARSER_INDEX++;
			lhs = parse_infix_expr(parser, index, TOKEN_SLASH, lhs);
		} else {
			break;
		}
	}
	return lhs;
}

void parse(Parser *parser) {
	Expression expr = parse_expression(parser, 0);
	*parser->expr = expr;
	if (PARSER_CURR_TOKEN == TOKEN_SEMI_COLON || PARSER_CURR_TOKEN == TOKEN_COMMA) {
		PARSER_INDEX++;
	}
}
