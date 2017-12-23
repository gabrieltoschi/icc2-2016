/**
	>>>> TOOLS <<<<
	Funcoes genericas para serem usadas em
	todo o programa.
**/

#include "tools.h"

#include <stdlib.h>
#include <string.h>

/** >> readString
	Le uma string de um determinado fluxo de dados.
**/
char *readString(FILE *stream){
	char *str = NULL;
	int ct = 0;
	char c;

	do {
		c = fgetc(stream);
		str = (char *) realloc(str, sizeof(char)*(ct+1));
		str[ct++] = c;
	} while (c != EOF);

	str[--ct] = '\0';
	return str;
}

/** >> frequenceReading
	Le a frequencia de cada caractere ASCII em uma string.
**/
int *frequenceReading(char *stream){
	int *frequences = (int *) calloc(ASCII_MAX, sizeof(int));
	int i, end = strlen(stream);
	
	for (i = 0; i < end; i++)
		frequences[(int) stream[i]]++;
		
	return frequences;
}

/** >> concatMalloc
	Concatena duas strings e aloca o resultado
	em uma nova terceira string.
**/
char *concatMalloc(char *str1, char *str2){
	int size = strlen(str1) + strlen(str2);
	char *cc = malloc(sizeof(char)*(size+1));
	
	strcpy(cc, str1);
	strcat(cc, str2);
	
	return cc;
}

/** >> checkExtension
	Checa a extensão de um arquivo a partir do seu nome.
**/
int checkExtension(char *filename){
	int i;

	while (filename[i] != '.') i++;

	if (strcmp(filename+i+1,   COMPRESS_EXT) == 0) return COMPRESS_FLAG;
	if (strcmp(filename+i+1, UNCOMPRESS_EXT) == 0) return UNCOMPRESS_FLAG;
	return NOCOMMAND_FLAG;
}

/** >> cleanString
	Atribui '\0' a todas as posicoes de uma string;
**/
void cleanString (char *str, int size){
	size--;
	while (size >= 0) str[size--] = (char) 0;
}

/** >> testDelimiter
	Retorna 1 caso seja encontrado um certo
	delimitador em um arquivo.
**/
int testDelimiter(FILE *inputFile, char delimiter){
	char test = fgetc(inputFile);
	int result = 0;

	if (test == delimiter) result = 1;
	ungetc(test, inputFile);

	return result;
}
