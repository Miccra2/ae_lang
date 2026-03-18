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

void token_info(Lexer *lexer) {
	size_t len = LEX_TOKENS.length;
	printf("toekns: %zu\n", len);
	for (size_t i = 0; i < LEX_TOKENS.length; i++) {
		Position pos = LEX_POSITION_BUFFER[i];
		TokenKind token  = LEX_TOKEN_BUFFER[i];
		printf("Info: %zu:%zu:%s: Token[%zu](kind: %s(%zu), Position(begin: %zu, end: %zu, offset: %zu, line: %zu))\n", 
			pos.line, position_column(pos), LEX_PATH, i, token_kind[(size_t)token], (size_t)token, 
			(size_t)(pos.begin - LEX_TEXT), (size_t)(pos.end - LEX_TEXT), (size_t)(pos.offset - LEX_TEXT), pos.line);
	}
}

int main(void) {
	char *path = "test.aec";
	Lexer lexer = new_lexer(path);
	tokenise(&lexer);
	token_info(&lexer);
	return 0;
}
