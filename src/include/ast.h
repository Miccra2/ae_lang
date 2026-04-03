#ifndef __AST_H__
#define __AST_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "token.h"

typedef struct Expression Expression;

typedef struct InfixExpr {
	TokenKind  kind;
	Expression *lhs;
	Expression *rhs;
} InfixExpr;

typedef enum ExprKind {
	EXPR_UNDEFINED,
	EXPR_ERROR,
	EXPR_IDENTIFIER,
	EXPR_VALUE,
	EXPR_INFIX,
} ExprKind;

struct Expression {
	ExprKind kind;
	int64_t  begin;
	int64_t  end;
	union {
		uint64_t  value;
		InfixExpr infix;
	};
};

extern char *expr_kind[];

Expression new_expression(ExprKind kind, int64_t begin, int64_t end, void *expr);
InfixExpr new_infix_expr(TokenKind kind, Expression lhs, Expression rhs);
void del_expression(Expression **expr);
void del_infix_expr(InfixExpr *infix);

#endif // __AST_H__
