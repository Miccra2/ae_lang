#include "lexer.h"

Lexer new_lexer(char *path) {
	FILE *pf = fopen(path, "r");
	if (!pf) {
		fprintf(stderr, "Error: %s: Could not open file!\n", path);
		abort();
	}
	fseek(pf, 0, SEEK_END);
	size_t size = ftell(pf);
	char *buffer = malloc(size * sizeof(char));
	if (!buffer) {
		fprintf(stderr, "Error: %s: Could not allocate text buffer!\n", path);
		abort();
	}
	rewind(pf);
	fread(buffer, sizeof(char), size, pf);
	fclose(pf);
	return (Lexer){
		.path      = path,
		.text      = buffer,
		.text_end  = buffer + size,
		.position  =  new_position(buffer, buffer, buffer, 1),
		.positions = (Positions)DA_EMPTY,
		.tokens    = (Tokens)DA_EMPTY,
	};
}

void del_lexer(Lexer *lexer) {
	if (lexer) {
		free(LEX_TEXT);
		LEX_TEXT = NULL;
		LEX_TEXT_EOF = NULL;
		if (LEX_POSITION_BUFFER) {
			free(LEX_POSITION_BUFFER);
			LEX_POSITIONS = (Positions)DA_EMPTY;
		}
		if (LEX_TOKEN_BUFFER) {
			free(LEX_TOKEN_BUFFER);
			LEX_TOKENS = (Tokens)DA_EMPTY;
		}
	}
}

bool text_ends(Lexer *lexer, size_t offset) {
	return LEX_CUR + offset >= LEX_TEXT_EOF;
}

bool has_char(Lexer *lexer, size_t offset, char *chars) {
	size_t len = strlen(chars);
	if (text_ends(lexer, offset)) {
		return false;
	}
	for (size_t i = 0; i < len; i++) {
		if (LEX_CUR[offset] == chars[i]) {
			return true;
		}
	}
	return false;
}

bool is_str(Lexer *lexer, size_t offset, char *str) {
	size_t len = strlen(str);
	if (text_ends(lexer, offset + len)) {
		return false;
	}
	size_t i = 0;
	for (; i < len; i++) {
		if (LEX_CUR[offset + i] != str[i]) {
			return false;
		}
	}
	return i == len;
}

Token generate_token(Lexer *lexer) {
	while (!text_ends(lexer, 0)) {
		if (has_char(lexer, 0, WHITESPACE)) {
			if (is_str(lexer, 0, NEWLINE)) {
				LEX_LINE++;
				LEX_OFFSET = LEX_CUR + 1;
			}
			LEX_CUR++;
		} else if (is_str(lexer, 0, INLINE_COMMENT)) {
			while (!text_ends(lexer, 0) && !is_str(lexer, 0, NEWLINE)) {
				LEX_CUR++;
			}
		} else if (is_str(lexer, 0, BLOCK_COMMENT_BEGIN)) {
			while (!text_ends(lexer, 0) && !is_str(lexer, 0, BLOCK_COMMENT_END)) {
				if (is_str(lexer, 0, NEWLINE)) {
					LEX_LINE++;
					LEX_OFFSET = LEX_CUR + 1;
				}
				LEX_CUR++;
			}
		} else {
			break;
		}
	}
	LEX_PTR = LEX_CUR;
	if (text_ends(lexer, 0)) {
		return new_token(TOKEN_EOF, LEX_POINT);
	}
	TokenKind kind = TOKEN_UNDEFINED;
	if (is_str(lexer, 0, "=")) {
		kind = TOKEN_EQUAL;
	} else if (is_str(lexer, 0, "+")) {
		kind = TOKEN_PLUS;
	} else if (is_str(lexer, 0, "-")) {
		kind = TOKEN_MINUS;
	} else if (is_str(lexer, 0, "*")) {
		kind = TOKEN_ASTERISK;
	} else if (is_str(lexer, 0, "/")) {
		kind = TOKEN_SLASH;
	} else if (is_str(lexer, 0, ".")) {
		kind = TOKEN_DOT;
	} else if (is_str(lexer, 0, ":")) {
		kind = TOKEN_COLON;
	} else if (is_str(lexer, 0, ",")) {
		kind = TOKEN_COMMA;
	} else if (is_str(lexer, 0, ";")) {
		kind = TOKEN_SEMI_COLON;
	} else if (is_str(lexer, 0, "@")) {
		kind = TOKEN_AT;
	} else if (is_str(lexer, 0, "#")) {
		kind = TOKEN_HASHTAG;
	} else if (is_str(lexer, 0, "'")) {
		kind = TOKEN_SINGLE_QUOTE;
	} else if (is_str(lexer, 0, "\"")) {
		kind = TOKEN_DOUBLE_QUOTE;
	} else if (is_str(lexer, 0, "(")) {
		kind = TOKEN_OPEN_PARENTHESE;
	} else if (is_str(lexer, 0, ")")) {
		kind = TOKEN_CLOSE_PARENTHESE;
	} else if (is_str(lexer, 0, "{")) {
		kind = TOKEN_OPEN_BRACE;
	} else if (is_str(lexer, 0, "}")) {
		kind = TOKEN_CLOSE_BRACE;
	} else if (is_str(lexer, 0, "[")) {
		kind = TOKEN_OPEN_BRACKET;
	} else if (is_str(lexer, 0, "]")) {
		kind = TOKEN_CLOSE_BRACKET;
	} else if (has_char(lexer, 0, DIGIT)) {
		while (has_char(lexer, 1, DIGIT)) {
			LEX_CUR++;
		}
		kind = TOKEN_INTEGER_LITERAL;
	} else if (has_char(lexer, 0, IDENTIFIER_BEGIN)) {
		while (has_char(lexer, 1, IDENTIFIER_END)) {
			LEX_CUR++;
		}
		kind = TOKEN_IDENTIFIER;
	} else {
		kind = TOKEN_ERROR;
	}
	LEX_CUR++;
	return new_token(kind, LEX_POINT);
}

void tokenise(Lexer *lexer) {
	Token token = generate_token(lexer);
	while (token.kind != TOKEN_EOF) {
		DA_PUSH(LEX_POSITIONS, token.position);
		DA_PUSH(LEX_TOKENS, token.kind);
		token = generate_token(lexer);
	}
}
