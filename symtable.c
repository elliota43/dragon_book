#include "symtable.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>

void symtab_init(SymbolTable *table) {
  table->entries = NULL;
  table->count = 0;
  table->capacity = 0;
}

Symbol *symtab_lookup_or_insert(SymbolTable *table, const char *name) {
  for (int i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].name, name) == 0) {
      return &table->entries[i];
    }
  }

  if (table->count == table->capacity) {
    int new_capacity = table->capacity == 0 ? 8 : table->capacity * 2;
    Symbol *grown =
        realloc(table->entries, (size_t)new_capacity * sizeof *grown);
    if (!grown) {
      return NULL;
    }

    table->entries = grown;
    table->capacity = new_capacity;
  }

  Symbol *sym = &table->entries[table->count++];
  sym->name = strdup(name);
  sym->kind = TOK_IDENTIFIER;
  return sym;
}

void symtab_free(SymbolTable *table) {
  for (int i = 0; i < table->count; i++) {
    free(table->entries[i].name);
  }

  free(table->entries);
  symtab_init(table);
}
