#ifndef LEXER_H
#define LEXER_H

#include "symtable.h"
#include "token.h"
#include <stdio.h>

typedef struct {
  FILE *source;
  int current_char;
  int line;
  SymbolTable *symtab;
} Lexer;

void lexer_init(Lexer *lexer, FILE *source, SymbolTable *symtab);
void lexer_advance_char(Lexer *lexer); // wraps getchar()
Token lexer_get_next_token(Lexer *lexer);

#endif
