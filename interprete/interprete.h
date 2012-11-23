#ifndef __INTERPRETE__
#define __INTERPRETE__

#include "hachage.h"

struct _symbole {
  char * name ;
  int val ;
} ;

#define NAME_SYMBOLE(s) ((s)->name)
#define VAL_SYMBOLE(s) ((s)->val)

typedef struct _symbole Symbole ;
extern HashTable tableSymboles ;
//extern Liste tableSymboles ;

int interprete_init () ;

#define insert_symbole(h, n, s) hashTable_add((h), (n), (s))
#define supprime_symbole(h, n) hashTable_rm((h), (n))
#define search_symbole(h, n) hashTable_search((h), (n))

#endif /* __INTERPRETE__ */
