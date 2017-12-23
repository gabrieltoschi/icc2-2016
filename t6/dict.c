#include <dict.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* le uma palavra de uma entrada e conta a frequencia de cada letra */
char *readWordWithFreq(FILE *stream, int *freq, int *wordSize){
	char *word = NULL;
	int ct = 0;
	char c;
	
	// enquanto nao chegar ao fim da linha/arquivo
	do {
		// pegando um caractere do arquivo
		c = fgetc(stream);
		// colocando caractere no vetor
		word = (char *) realloc(word, sizeof(char) * (ct + 1));
		word[ct++] = c;
		
		// se nao for os caracteres de parada
		if (c != '\n' && c != EOF){
			// somar 1 no vetor de frequencia
			if (c == ASCII_HYPHEN) freq[LETTER_MAX - 1]++;
				else freq[c - ASCII_LETTER_CONV]++;
		}		
	} while (c != '\n' && c != EOF);
	
	// colocando terminador de parada na string
	word[ct-1] = '\0';
	
	*wordSize = ct-1;
	return word;
}

/* le todas as palavras de uma entrada e armazena em um dicionario */
Dictionary *readWords(char *inputFilename){
	char *wordAux;
	int wordSizeAux;
	
	// inicializando o dicionario
	Dictionary *dict = (Dictionary *) malloc(sizeof(Dictionary));
	dict->words = NULL;
	dict->wordCount = 0;
	dict->frequences = (int *) calloc(LETTER_MAX, sizeof(int));
	
	// abrindo o arquivo
	FILE *stream = fopen(inputFilename, "r");
	
	// enquanto o arquivo nao acabar
	while (!feof(stream)){
		wordAux = readWordWithFreq(stream, dict->frequences, &wordSizeAux);
		if (!feof(stream)){
			dict->words = (Word *) realloc(dict->words, 
									sizeof(Word) * (dict->wordCount + 1));
			dict->words[dict->wordCount].data = wordAux;
			dict->words[dict->wordCount++].size = wordSizeAux;
		} else {
			// o arquivo acabou
			free(wordAux);
		}
	}
	
	fclose(stream);
	return dict;
}

/* libera um dicionario da memoria heap */
void freeDictionary(Dictionary *d){
	// liberando cada palavra do dicionario
	int i;
	for (i = 0; i < d->wordCount; i++)
		free(d->words[i].data);
	free(d->words);

	// liberando vetor de frequencias
	free(d->frequences);

	// liberando dicionario
	free(d);
}

/* gera um vetor de numeros primos na memoria heap */
Prime *getPrimeArray(){
	// cria vetor base com os numeros primos
	const Prime source[LETTER_MAX] =
		{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
		53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103};
	
	// aloca vetor para guardar todos os primos usados
	Prime *primes = (Prime *) malloc(sizeof(Prime) * LETTER_MAX);
		
	// copia valores da base para a memoria heap
	int i;
	for (i = 0; i < LETTER_MAX; i++) primes[i] = source[i];
	
	return primes;
}

/* gera uma chave unica de anagrama para uma palavra */
AnagramKey getAnagramKey(Prime* primes, int *primeConv, Word *word){
	AnagramKey key = 1;

	int i;

	// para cada letra da palavra
	for (i = 0; i < word->size; i++)
		// multiplique na chave o seu numero primo correspondente
		key *= primes[primeConv[word->data[i] - ASCII_LETTER_CONV]];

	return key;
}

/* gera chaves unicas de anagramas para todas as palavras do dicionario */
void generateAnagramKeys(Prime *primes, int *primeConv, Dictionary *dict){
	int i;

	// para cada palavra do dicionario
	for (i = 0; i < dict->wordCount; i++)
		// gere a chave unica de anagrama para ela
		dict->words[i].key = getAnagramKey(primes, primeConv, dict->words+i);
}

/* cria um dicionario de anagramas */
AnagramDict *createAnagramDict(){
	// alocando dicionario de anagramas
	AnagramDict *aDict = (AnagramDict *) calloc(1, sizeof(AnagramDict));
	aDict->anagramMax = 1;

	return aDict;
}

/* libera um dicionario de anagramas */
void freeAnagramDict(AnagramDict *aDict){
	int i;
	for (i = 0; i < aDict->anagramCount; i++){
		free(aDict->anagrams[i]->words);
		free(aDict->anagrams[i]);
	}

	if (aDict->anagrams != NULL) free(aDict->anagrams);
	free(aDict);
}

/* funcoes para ordenacao de palavras usando
	o algoritmo quicksort */
void swapWord(Word **v, int a, int b){
	Word *aux = v[a];
	v[a] = v[b];
	v[b] = aux;
}

int partitionWord(Word **v, int l, int r){
	int i, j;
	
	i = l;
	for (j = i+1; j <= r; j++){
		if (strcmp(v[j]->data, v[l]->data) < 0){
			++i;
			swapWord(v, i, j);
		}
	}
	swapWord(v, i, l);
	
	return i;
}

void quicksortWord(Word **v, int l, int r){
	int c;
	
	if (l < r){
		c = partitionWord(v, l, r);
		quicksortWord(v, l, c-1);
		quicksortWord(v, c+1, r);
	}
}

/* funcoes para ordenacao de grupos de anagramas usando
	o algoritmo quicksort */
void swapGroup(AnagramGroup **v, int a, int b){
	AnagramGroup *aux = v[a];
	v[a] = v[b];
	v[b] = aux;
}

int partitionGroup(AnagramGroup **v, int l, int r){
	int i, j;
	
	i = l;
	for (j = i+1; j <= r; j++){
		if (strcmp(v[j]->words[0]->data, v[l]->words[0]->data) < 0){
			++i;
			swapGroup(v, i, j);
		}
	}
	swapGroup(v, i, l);
	
	return i;
}

void quicksortGroup(AnagramGroup **v, int l, int r){
	int c;
	
	if (l < r){
		c = partitionGroup(v, l, r);
		quicksortGroup(v, l, c-1);
		quicksortGroup(v, c+1, r);
	}
}