#include "ast.h"

char *expr_kind[] = {
	"UNDEFINED",
	"ERROR",
	"IDENTIFIER",
	"VALUE",
	"INFIX",
};

Expression new_expression(ExprKind kind, int64_t begin, int64_t end, void *expr) {
	Expression __expr;
	__expr.kind  = kind;
	__expr.begin = begin;
	__expr.end   = end;
	switch (kind) {
	case EXPR_VALUE: __expr.value = *(uint64_t*)expr;  break;
	case EXPR_INFIX: __expr.infix = *(InfixExpr*)expr; break;
	default:                                           break;
	}
	return __expr;
}

InfixExpr new_infix_expr(TokenKind kind, Expression lhs, Expression rhs) {
	InfixExpr infix;
	infix.lhs    = malloc(sizeof(Expression));
	infix.rhs    = malloc(sizeof(Expression));
	infix.kind   = kind;
	*(infix.lhs) = lhs;
	*(infix.rhs) = rhs;
	return infix;
}

void del_expression(Expression **expr) {
	if (expr) {
		free(*expr);
		*expr = NULL;
	}
}

void del_infix_expr(InfixExpr *infix) {
	if (infix) {
		if (infix->lhs) {
			free(infix->lhs);
			infix->lhs = NULL;
		}
		if (infix->rhs) {
			free(infix->rhs);
			infix->lhs = NULL;
		}
	}
}
