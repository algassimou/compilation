#ifndef __HACHAGE__
#define __HACHAGE__

#include <stdint.h>

#include "liste.h"
#include "couple.h"
/**
 * Definition du type de la table de hachage 
 * celle ci est un tableau de liste (car les collisions sont gérer en chainant 
 * les différents couple qui sont entrés en collision
 */

struct _HashTable {
  Liste **table ;
  Liste *liste_listes_collision ;
  int size ; // la taille du tableau>
  int count ;// le nombre de couple contenu dans le tableau 
  void (*destroy_cle) (void*); // fonction pour liberer l'espace mémoire occupé par une clé
  void (*destroy_val) (void*); // fonction pour liberer l'espace mémoire occupé par une valeur
  /* 
   * fonction de comapraison des clés
   * renvoi :
   * 0 si les deux clés sont égales
   * 1 sinon 
   */
  int (*compare) (const void *, const void *); 
  uint32_t (*hash_function) (const void *);
} ;

#define HASH_TABLE_SIZE(h) ((h)->size)
#define HASH_TABLE_COUNT(h) ((h)->count)
#define HASH_TABLE_SIZE_MAX 1024

typedef struct _HashTable HashTable ;

/**
 * initialise une table de hachage
 * \param h la table de hachage 
 * \param hash la donction de hachage utilisée
 * \param cmp fonction de comparaison
 * \param dc une fonction pour la gestion mémoire des clés
 * \param dv une fonction pour la gestion mémoire des valeurs
 */
int hashTable_init (HashTable *h, 
		    uint32_t (*hash) (const void *),
		    int (*cmp) (const void *, const void *),
		    void (*dc)(void*), void (*dv)(void*)) ;

/**
 * ajoute une valeur dans la table de hachage 
 * \param h la table de hachage 
 * \param cle la cle de la valeur 
 * \param val la valeur
 */
int hashTable_add (HashTable *h, void *cle, void *val);

/**
 * recherche la valeur d'une cle dans une table de hachage
 * \param h la table de hachage 
 * \param cle la cle
 * \result renvoie le couple formé par la cle et la valeur si la cle est 
 * presente dans la table sinon renvoie NULL
 */
Couple* hashTable_search (HashTable *h, void *cle);

/**
 * supprime une valeur de la table
 * \param h la table de hachage
 * \param cle la cle de la valeur
 * \result renvoie  si tous c'est bien passé 
 */
int hashTable_rm (HashTable *h, void *cle);

/**
 * detruit la table de hachage ainsi toutes les clés et valeur quelle contient
 * les fonctions destroy_cle et destroy_val sont utilisées pour liberer 
 * l'espace mémoire occupé par les clés et les valeurs
 * \param h l table de hachage
 */
int hashTable_destroy(HashTable *h);

#endif 
