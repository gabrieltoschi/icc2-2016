/**
	>>>> HUFFMAN <<<<
	Funcoes usadas para compactacao
	e decompactacao de Huffman.
**/

#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "tools.h"
#include "lists.h"
#include "trees.h"

#define HUFFTABLE_READ_DELIMITER '-'
#define COMPRESSED_TEXT_DELIMITER EOF

Huff *createCharHuff(char ch, int freq);
Huff *createStrHuff(char *str, int freq);
void printHuff(Huff *huff);
void freeHuff(Huff *huff);
List *createFrequenceList(char *chars, int *frequences, int charCount);
Tree *huffmanMakeTree(List *chars);
HuffTable *huffmanMakeTable(Tree *tree);
void huffmanPrintTable(HuffTable *table);
char *sortFrequences(int *freq);
int getCharCount(int *freq);
void sortHuffTableSymbol(HuffTable *table);
void huffmanCompress(char *text, HuffTable *table);
void destroyHuffTable(HuffTable *t);
HuffTable *readHuffTable(FILE *inputFile);
unsigned char *readCompressedText(FILE *inputFile, int *inputSize);
void sortHuffTableCode(HuffTable *table);
void huffmanUncompress(unsigned char *text, int byteSize, HuffTable *table);
	
#endif
