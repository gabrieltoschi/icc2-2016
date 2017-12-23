#include <stdlib.h>
#include <stdio.h>

#include <dict.h>
#include <utils.h>
#include <hash.h>

int main(int argc, char *argv[]){
	int a, b;

	// lendo o nome do arquivo com as palavras
	char *inputFilename;
	scanf("%ms", &inputFilename);

	// lendo as palavras do arquivo e formando o dicionario
	Dictionary *dict = readWords(inputFilename);

	// gerando um vetor de primos na memoria heap
	Prime *primes = getPrimeArray();

	// gerando tabela de conversao letra->primo, a partir
	// das frequencias de cada letra no dicionario
	int *primeConv = sortByFreq(dict->frequences, LETTER_MAX);

	// gerando as chaves unicas de anagramas a partir
	// da multiplicacao de numeros primos
	generateAnagramKeys(primes, primeConv, dict);

	// inicializando tabela hash
	HashTable *table = hashCreate(WORDS_HASH_SIZE);

	// colocando cada palavra do dicionario na tabela hash
	for (a = 0; a < dict->wordCount; a++)
		hashInsertWord(table, dict->words + a);

	// inicializando dicionario de anagramas
	AnagramDict *aDict = createAnagramDict();

	// recuperando dicionario de anagramas
	for (a = 0; a < table->size; a++)
		if (table->units[a].isFull && !table->units[a].isGet)
			aDict = hashGetGroup(table, table->units[a].word->key, aDict);

	// ordenando cada um dos grupos de anagramas
	for (a = 0; a < aDict->anagramCount; a++)
		quicksortWord(aDict->anagrams[a]->words, 0, aDict->anagramMax - 1);

	// ordenando o dicionario de anagramas
	quicksortGroup(aDict->anagrams, 0, aDict->anagramCount - 1);

	// imprimindo anagramas
	printf("%d\n", aDict->anagramCount);
	for (a = 0; a < aDict->anagramCount; a++){
		for (b = 0; b < aDict->anagrams[a]->counter - 1; b++)
			printf("%s, ", aDict->anagrams[a]->words[b]->data);
		printf("%s\n", aDict->anagrams[a]->words[aDict->anagrams[a]->counter - 1]->data);
	}

	// liberando memoria heap alocada
	free(inputFilename);
	freeDictionary(dict);
	free(primes);
	free(primeConv);
	hashDestroy(table);
	freeAnagramDict(aDict);
	return 0;
}