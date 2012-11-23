#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hachage.h"


static uint32_t hash_address (const void *addr)
{
  assert (addr != NULL);

  register uint32_t key;
  key = (uint32_t) addr;
  return (key >> 3) * 2654435761 ;
}

int hashTable_init (HashTable *h, 
		    uint32_t (*hash) (const void *),
		    int (*cmp) (const void *, const void *),
		    void (*dc)(void*), void (*dv)(void*)) {

  assert (h != NULL);
  assert (cmp != NULL);

  if ((h -> table = malloc (HASH_TABLE_SIZE_MAX * sizeof (Liste*))) == NULL)
    return -1 ;

  if ((h->liste_listes_collision = malloc (sizeof(Liste))) == NULL)
    return -1 ;
  
  liste_init (h->liste_listes_collision, NULL);

  h->size = HASH_TABLE_SIZE_MAX ;
  h->compare = cmp ;
  h->destroy_cle = dc ;
  h->destroy_val = dv ;
  h->count = 0 ;

  if (hash == NULL)
    h->hash_function = hash_address ;
  else
    h->hash_function = hash ;
  
  return 0 ;
}

int hashTable_add (HashTable *h, void *cle, void *val) {
  assert (h != NULL);
  assert (cle != NULL);

  uint32_t key = h->hash_function (cle) ;
  Liste * collision = h->table[key % h->size] ;
  if (collision == NULL) {
    if ((collision = malloc (sizeof(Liste))) == NULL)
      return -1 ;

    liste_init (collision, NULL) ;
    if (liste_add (h->liste_listes_collision, LISTE_QUEUE(h->liste_listes_collision), collision) == -1 )
      return -1 ;

    h->table [key % h->size] = collision ;

    Couple * c = NULL ;
    if ((c = malloc (sizeof(Couple))) == NULL)
      return -1 ;

    couple_init (c, cle, val);

    if (liste_add (collision, NULL, c) == -1)
      return -1 ;

    h->count++ ;
    return 0 ;
  }

  else {
    Couple * c = hashTable_search(h, cle);
    if ( c != NULL) {
      if (h->destroy_val)
	h->destroy_val(COUPLE_SECOND(c));
      
      COUPLE_SECOND(c) = val ;
      return 0 ;
    }
   
    if ((c = malloc (sizeof(Couple))) == NULL)
      return -1 ;

    couple_init (c, cle, val);

    if (liste_add (collision, NULL, c) == -1)
      return -1 ;

    h->count++ ;
    return 0 ;
  }
}

Couple *hashTable_search (HashTable *h, void *cle){
  assert (h != NULL);
  assert (cle != NULL);

  uint32_t key = h->hash_function (cle) ;
  Liste * collision = h->table[key % h->size] ;
  if (collision == NULL) 
    return NULL ;
  
  ListElem * elt = NULL ;
  elt = LISTE_TETE (collision) ;
  Couple * c = NULL ;
    
  while (elt) {
    c = (Couple *) LISTE_ELEM_DATA(elt) ;
    if (h->compare(COUPLE_FIRST(c), cle) == 0)
      return c ;

    elt = LISTE_ELEM_NEXT(elt);
  } 
  return NULL ;
}

int hashTable_destroy(HashTable *h) {
  assert(h != NULL);
  
  ListElem *liste_elt = LISTE_TETE(h->liste_listes_collision);

  while (liste_elt) {
    Liste *liste = (Liste *) LISTE_ELEM_DATA(liste_elt) ;
    ListElem *elt = LISTE_TETE(liste) ;

    while (elt) {
      Couple *c = (Couple *) LISTE_ELEM_DATA(elt) ;
      if (h->destroy_cle)
	h->destroy_cle(COUPLE_FIRST(c));

      if (h->destroy_val)
	h->destroy_val(COUPLE_SECOND(c));

      free(c);

      elt = LISTE_ELEM_NEXT(elt);
      h->count -- ;
    }
    liste_elt = LISTE_ELEM_NEXT(liste_elt);
    free(liste);
  }

  return 0 ;
}

int hashTable_rm (HashTable *h, void *cle) {
  assert (h != NULL);
  assert (cle != NULL);

  if (h->count == 0)
    return 1 ;

  uint32_t key = h->hash_function (cle) ;
  Liste * collision = h->table[key % h->size] ;
  if (collision == NULL) 
    return 1 ;
  
  ListElem *elt = NULL ;
  ListElem *prev = NULL ;
  elt = LISTE_TETE (collision) ;
  Couple * c = NULL ;
    
  while (elt) {
    c = (Couple *) LISTE_ELEM_DATA(elt) ;
    if (h->compare(COUPLE_FIRST(c), cle) == 0)
      {
	if (h->destroy_cle)
	  h->destroy_cle(COUPLE_FIRST(c));

	if (h->destroy_val)
	  h->destroy_val(COUPLE_SECOND(c));

	free(c);
	h->count -- ;
	return liste_rm (collision, prev, NULL);
      }

    prev = elt ;
    elt = LISTE_ELEM_NEXT(elt);
  } 
  return 1 ;
}
