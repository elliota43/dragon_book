#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOK_EOF,
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,
  TOK_SEMICOLON,

  TOK_LPAREN,
  TOK_RPAREN,
  TOK_NUMBER,
  TOK_IDENTIFIER,

  TOK_DIV,
  TOK_MOD,

  TOK_ERROR,
  TOK_UNKNOWN,

  TOK_COUNT,
} TokenKind;

typedef struct {
  char *name; // for identifiers
  int value;  // for numbers
  int line;   // for error reporting
  TokenKind kind;
} Token;

#endif
