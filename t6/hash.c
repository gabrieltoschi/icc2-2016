#include <hash.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dict.h>
#include <utils.h>

/* cria uma hash table */
HashTable *hashCreate(int size){
	// alocando memoria para a tabela hash
	HashTable *table = (HashTable *) malloc(sizeof(HashTable));

	// inicializando tabela hash
	table->units = (HashUnit *) calloc(size, sizeof(HashUnit));
	table->size = size;

	return table;
}

/* libera uma hash table da memoria heap */
void hashDestroy(HashTable *table){
	// liberando tabela hash da memoria heap
	free(table->units);
	free(table);
}

/* gera uma id para acesso na hash table a partir do
   metodo da divisao */
HashID hashDivision(HashTable *table, AnagramKey key){
	return key % table->size;
}

/* insere uma palavra na hash table */
void hashInsertWord(HashTable *table, Word *word){
	int loopCounter = 0;

	// gerando primeira id possivel da palavra na tabela hash
	HashID id = hashDivision(table, word->key);

	// enquanto nao houver uma posicao vazia e enquanto nao
	// checar toda a tabela hash
	while (++loopCounter < table->size && table->units[id].isFull)
		// cheque a proxima posicao da tabela
		// (espalhamento linear)
		id = hashDivision(table, id+1);

	// se a posicao encontrada estiver vazia
	if (!table->units[id].isFull){
		table->units[id].word = word;
		table->units[id].isFull = true;
	}
}

/* procura por um grupo de anagramas na hash table,
   a partir de uma chave unica de anagrama */
AnagramDict *hashGetGroup(HashTable *table, AnagramKey key, AnagramDict *aDict){
	int loopCounter = 0;

	// alocando grupo de anagramas
	AnagramGroup *group = calloc(1, sizeof(AnagramGroup));

	// gerando id do conjunto de entradas da tabela hash relacionados
	// a chave que esta buscando
	HashID id = hashDivision(table, key);

	// enquanto nao houver uma posicao vazia e enquanto nao
	// checar toda a tabela hash
	while (++loopCounter < table->size && table->units[id].isFull){
		// se essa entrada da tabela hash tiver uma palavra
		// com a chave buscada e ela nao foi pega antes
		if (!table->units[id].isGet && table->units[id].word->key == key){
			// armazene ele no grupo de anagramas
			group->words = (Word **) realloc(group->words,
				sizeof(Word *) * (group->counter + 1));
			group->words[group->counter++] = table->units[id].word;
			table->units[id].isGet = true;
		}

		// gere o novo id para continuar procurando
		id = hashDivision(table, id+1);
	}

	// caso algum anagrama seja encontrado com essa chave
	// e o conjunto de anagramas seja igual ao maximo tamanho
	// do conjunto
	if (group->counter == aDict->anagramMax){
		// coloque esse grupo no dicionario atual de anagramas
		aDict->anagrams = (AnagramGroup **) realloc(aDict->anagrams,
			sizeof(AnagramGroup *) * (aDict->anagramCount + 1));
		aDict->anagrams[aDict->anagramCount++] = group;

	// caso o conjunto de anagramas tenha tamanho maior que o maximo
	} else if (group->counter > aDict->anagramMax){
		// libere o dicionario atual de anagramas
		freeAnagramDict(aDict);

		// crie outro dicionario, com um novo maximo
		aDict = createAnagramDict();
		aDict->anagramMax = group->counter;

		// coloque esse grupo no dicionario atual de anagramas
		aDict->anagrams = (AnagramGroup **) realloc(aDict->anagrams,
			sizeof(AnagramGroup *) * (aDict->anagramCount + 1));
		aDict->anagrams[aDict->anagramCount++] = group;
	} else {
		// esse conjunto de anagramas nao e util
		// libere essa memoria alocada
		if (group->words != NULL) free(group->words);
		free(group);
	}

	return aDict;
}

