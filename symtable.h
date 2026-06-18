#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "token.h"

typedef struct {
  char *name;
  TokenKind kind; // TOK_IDENTIFIER, TOK_DIV, TOK_MOD, etc.
} Symbol;

typedef struct {
  Symbol *entries;
  int count;
  int capacity;
} SymbolTable;

void symtab_init(SymbolTable *table);
Symbol *symtab_lookup_or_insert(SymbolTable *table, const char *name);
void symtab_free(SymbolTable *table);

#endif
