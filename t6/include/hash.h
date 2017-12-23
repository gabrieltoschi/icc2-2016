#ifndef _HASH_H_
#define _HASH_H_

#include <dict.h>

/* HashID: indice de acesso a uma entrada de uma hash table*/
typedef unsigned int HashID;

/* HashUnit: entrada de hash table */
typedef struct hashunit {
	Word *word;
	unsigned char isFull:1;
	unsigned char isGet:1;
} HashUnit;

/* HashTable: define a estrutura de uma hash table */
typedef struct hashtable {
	HashUnit *units;
	int size;
} HashTable;

HashTable *hashCreate(int size);
void hashDestroy(HashTable *table);
void hashInsertWord(HashTable *table, Word *word);
AnagramDict *hashGetGroup(HashTable *table, AnagramKey key, AnagramDict *aDict);

#endif