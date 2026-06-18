#include "lexer.h"
#include "symtable.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void lexer_init(Lexer *lexer, FILE *source, SymbolTable *symtab) {
  lexer->source = source;
  lexer->symtab = symtab;
  lexer->line = 1;
  lexer->current_char = fgetc(source);
}

void lexer_advance_char(Lexer *lexer) {
  lexer->current_char = fgetc(lexer->source);

  if (lexer->current_char == '\n') {
    lexer->line++;
  }
}

Token lexer_get_next_token(Lexer *lexer) {
  while (lexer->current_char == ' ' || lexer->current_char == '\t' ||
         lexer->current_char == '\n') {
    lexer_advance_char(lexer);
  }

  if (lexer->current_char == EOF) {
    return (Token){.kind = TOK_EOF, .line = lexer->line};
  }

  switch (lexer->current_char) {
  case '+':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_PLUS, .line = lexer->line};
  case '-':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_MINUS, .line = lexer->line};
  case '*':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_STAR, .line = lexer->line};
  case '/':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_SLASH, .line = lexer->line};
  case '(':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_LPAREN, .line = lexer->line};
  case ')':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_RPAREN, .line = lexer->line};
  case ';':
    lexer_advance_char(lexer);
    return (Token){.kind = TOK_SEMICOLON, .line = lexer->line};
  }

  if (isdigit(lexer->current_char)) {
    int value = 0;
    while (isdigit(lexer->current_char)) {
      value = (value * 10) + (lexer->current_char - '0');
      lexer_advance_char(lexer);
    }
    return (Token){.kind = TOK_NUMBER, .value = value, .line = lexer->line};
  }

  if (isalpha(lexer->current_char)) {
    char buffer[256];
    int length = 0;
    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
      if (length < 255) {
        buffer[length++] = (char)lexer->current_char;
      }
      lexer_advance_char(lexer);
    }
    buffer[length] = '\0';

    Symbol *sym = symtab_lookup_or_insert(lexer->symtab, buffer);
    return (Token){
        .kind = sym->kind,
        .name = sym->name,
        .line = lexer->line,
    };
  }

  Token unknown_token = {.kind = TOK_UNKNOWN, .line = lexer->line};
  lexer_advance_char(lexer);
  return unknown_token;
}
