#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "token.h"

typedef struct Lexer {
	char      *path;
	char      *text;
	char      *text_end;
	Position  position;
	Positions positions;
	Tokens    tokens;
} Lexer;

// macros with strings for use with has_char and is_str functions
// used with is_str function
#define NEWLINE             "\n"
#define WHITESPACE          " \n\r\t"
#define INLINE_COMMENT      "//"
#define BLOCK_COMMENT_BEGIN "/*"
#define BLOCK_COMMENT_END   "*/"
// used with has_char function
#define DIGIT               "0123456789"
#define UPPER               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER               "abcdefghijklmnopqrstuvwxyz"
#define ALPHA               UPPER LOWER
#define IDENTIFIER_BEGIN    "_" ALPHA
#define IDENTIFIER_END      DIGIT IDENTIFIER_BEGIN

// macros for easier use (functions expected to pass lexer as pointer)
#define LEX_PATH            (lexer->path)
#define LEX_TEXT            (lexer->text)
#define LEX_TEXT_EOF        (lexer->text_end)
#define LEX_POINT           (lexer->position)
#define LEX_BEGIN           (lexer->position.begin)
#define LEX_END             (lexer->position.end)
#define LEX_OFFSET          (lexer->position.offset)
#define LEX_LINE            (lexer->position.line)
#define LEX_COLUMN          (position_column(lexer->position))
#define LEX_LENGTH          (position_length(lexer->position))
#define LEX_POSITIONS       (lexer->positions)
#define LEX_POSITION_BUFFER (lexer->positions.buffer)
#define LEX_TOKENS          (lexer->tokens)
#define LEX_TOKEN_BUFFER    (lexer->tokens.buffer)

// redefinition of macros for better readability
#define LEX_CUR             (LEX_END)
#define LEX_PTR             (LEX_BEGIN)

Lexer new_lexer(char *path);
void del_lexer(Lexer *lexer);
bool text_ends(Lexer *lexer, size_t offset);
bool has_char(Lexer *lexer, size_t offset, char *chars);
bool is_str(Lexer *lexer, size_t offset, char *chars);
Token generate_token(Lexer *lexer);
void tokenise(Lexer *lexer);

#endif // __LEXER_H__
