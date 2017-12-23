#ifndef _DICT_H_
#define _DICT_H_

#define LETTER_MAX 27
#define ASCII_LETTER_CONV 'a'
#define ASCII_HYPHEN '-'
#define WORDS_HASH_SIZE 300007

/* AnagramKey: chave unica de anagrama */
typedef unsigned long long int AnagramKey;
/* Prime: numero primo */
typedef unsigned char Prime;

/* Word: palavra de um dicionario */
typedef struct word {
	char *data;
	int size;
	AnagramKey key;
} Word;

/* Dictionary: conjunto de palavras, com vetor de frequencias */
typedef struct dictionary {
	Word *words;
	int wordCount;
	int *frequences;
} Dictionary;

/* AnagramGroup: grupo de anagramas entre si */
typedef struct anagramgroup {
	Word **words;
	int counter;
} AnagramGroup;

/* AnagramDict: dicionario de grupos de anagramas entre si */
typedef struct anagramdict {
	AnagramGroup **anagrams;
	int anagramCount;
	int anagramMax;
} AnagramDict;

Dictionary *readWords(char *inputFilename);
void freeDictionary(Dictionary *d);
Prime *getPrimeArray();
void generateAnagramKeys(Prime *primes, int *primeConv, Dictionary *dict);
AnagramDict *createAnagramDict();
void freeAnagramDict(AnagramDict *aDict);
void quicksortWord(Word **v, int l, int r);
void quicksortGroup(AnagramGroup **v, int l, int r);

#endif