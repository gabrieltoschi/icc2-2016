/**
	>>>> UTILS <<<<
	Define funcoes e tipos genericos que 
	auxiliam em varias funcoes.
**/

#include <stdio.h>
#include <stdlib.h>

/** >> freeDoublePointerSize
	Libera, da memoria heap, um ponteiro duplo,
	sabendo o seu tamanho.
**/
void freeDoublePointerSize(void **pointer, int size){
	size--;
	while (size >= 0) free(pointer[size--]);
	free(pointer);
}

/** >> freeDoublePointerTerm
	Libera, da memoria heap, um ponteiro duplo,
	ate que seja encontrado um endereco nulo no vetor,
	que funciona como "terminador".
**/
void freeDoublePointerTerm(void **pointer){
	int i = 0;
	for (i = 0; pointer[i] != NULL; i++) free(pointer[i]);
	free(pointer);
}

/** >> readLine
	Le uma linha digitada ate encontrar o indicador 
	de pulo de linha ou o fim do arquivo (EOF), a partir
	de um fluxo de dados
**/
char *readLine(FILE *stream, char delim){
	char *str = NULL;
	int counter = 0;
	char c;

	do {
		c = fgetc(stream);
		str = (char *) realloc(str, sizeof(char)*(counter+1));
		str[counter++] = c;
	} while (c != delim && c != EOF);

	str[--counter] = '\0';
	return str;
}

/** >> readAllLines
	Le todas as linhas digitadas em um arquivo,
	utilizando readLine. A ultima posicao do vetor 
	de strings contem NULL, indicando que nao
	ha mais linhas.
**/
char **readAllLines(FILE *stream){
	char **allLines = NULL;
	char *reading;
	int counter = 0, i;

	do {
		reading = readLine(stream, '\n');
		if (!feof(stream)){
			allLines = (char **) realloc(allLines, sizeof(char *) * (counter+1));
			allLines[counter] = reading;
			counter++;
		} else free(reading);
	} while (!feof(stream));

	allLines = (char **) realloc(allLines, sizeof(char *) * (counter+1));
	allLines[counter] = NULL;

	return allLines;
}