#include "lexer.h"
#include "symtable.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

static const char *token_kind_name(TokenKind kind) {
  switch (kind) {
  case TOK_EOF:
    return "EOF";
  case TOK_PLUS:
    return "PLUS";
  case TOK_MINUS:
    return "MINUS";
  case TOK_STAR:
    return "STAR";
  case TOK_SLASH:
    return "SLASH";
  case TOK_LPAREN:
    return "LPAREN";
  case TOK_RPAREN:
    return "RPAREN";
  case TOK_NUMBER:
    return "NUMBER";
  case TOK_IDENTIFIER:
    return "IDENTIFIER";
  case TOK_DIV:
    return "DIV";
  case TOK_MOD:
    return "MOD";
  case TOK_ERROR:
    return "ERROR";
  case TOK_UNKNOWN:
    return "UNKNOWN";
  default:
    return "?";
  }
}

int main(int argc, char **argv) {
  FILE *source = (argc > 1) ? fopen(argv[1], "r") : stdin;
  if (!source) {
    perror("fopen");
    return 1;
  }

  SymbolTable symtab;
  symtab_init(&symtab);

  symtab_lookup_or_insert(&symtab, "div")->kind = TOK_DIV;
  symtab_lookup_or_insert(&symtab, "mod")->kind = TOK_MOD;

  Lexer lexer;
  lexer_init(&lexer, source, &symtab);

  Token tok;
  do {
    tok = lexer_get_next_token(&lexer);
    printf("%4d  %-11s", tok.line, token_kind_name(tok.kind));
    if (tok.kind == TOK_NUMBER) {
      printf("  %d", tok.value);
    } else if (tok.name) {
      printf("  %s", tok.name);
    }
    putchar('\n');
  } while (tok.kind != TOK_EOF);

  if (source != stdin) {
    fclose(source);
  }

  symtab_free(&symtab);
  return 0;
}
