/**
	>>>> TOOLS <<<<
	Funcoes genericas para serem usadas em
	todo o programa.
**/

#ifndef _TOOLS_H_
#define _TOOLS_H_

#define ASCII_MAX 128
#define HUFF_BITS_MAX 8

#define NOCOMMAND_FLAG -1
#define COMPRESS_FLAG 0
#define COMPRESS_EXT "txt"
#define UNCOMPRESS_FLAG 1
#define UNCOMPRESS_EXT "huff"

#include <stdio.h>

typedef struct sHuff Huff;
typedef char Bit;
typedef struct sHuffUnit HuffUnit;
typedef struct sHuffTable HuffTable;

struct sHuff {
	char *string;
	int frequence;
};

struct sHuffUnit {
	char symbol;
	Bit *code;
};

struct sHuffTable { 
	HuffUnit *fields;
	int size;
};

char *readString(FILE *stream);
int *frequenceReading(char *stream);
char *concatMalloc(char *str1, char *str2);
int checkExtension(char *filename);
void cleanString(char *str, int size);
int testDelimiter(FILE *inputFile, char delimiter);

#endif
