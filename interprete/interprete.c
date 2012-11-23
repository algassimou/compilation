#include <string.h>
#include <stdlib.h>
#include "interprete.h"

HashTable tableSymboles ;

static int cmp (const void *c1, const void *c2) {
  char *s1 = (char *) c1 ;
  char *s2 = (char *) c2;

  return strcmp(s1,s2);
}

static uint32_t sdbm(const void *s)
{
  unsigned char *str = (unsigned char *) s ;
  uint32_t hash = 0;
  uint32_t c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

static void symbole_destroy (void *s) {
  Symbole *sym = (Symbole *) s ;
  free (sym->name);
  free (sym);
}

int interprete_init () {
  hashTable_init (&tableSymboles, sdbm, cmp, NULL, symbole_destroy);
}
