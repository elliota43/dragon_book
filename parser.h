#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"

typedef struct {
  Token current_token;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);
void parser_match(Parser *parser, TokenKind expected_token);

#endif
