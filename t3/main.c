#include <stdlib.h>
#include <stdio.h>

#include "tools.h"
#include "lists.h"
#include "huffman.h"
#include "trees.h"

void compress(FILE *inputFile){
	char *text = readString(inputFile);
	int *frequences = frequenceReading(text);
	char *chars = sortFrequences(frequences);
	int charCount = getCharCount(frequences);
	
	List *symbolList = createFrequenceList(chars, frequences, charCount);
	Tree *huffTree = huffmanMakeTree(symbolList);
	listFree(symbolList);

	HuffTable *huffTable = huffmanMakeTable(huffTree);
	sortHuffTableSymbol(huffTable);
	huffmanPrintTable(huffTable);
	huffmanCompress(text, huffTable);

	free(chars);
	free(frequences);
	free(text);
	treeDestroy(huffTree);
	destroyHuffTable(huffTable);
}

void uncompress(FILE *inputFile){
	HuffTable *huffTable = readHuffTable(inputFile);
	int compressedSize;
	sortHuffTableCode(huffTable);

	unsigned char *compressedText = readCompressedText(inputFile, &compressedSize);

	huffmanUncompress(compressedText, compressedSize, huffTable);
	destroyHuffTable(huffTable);
	free(compressedText);
}

int main (int argc, char *argv[]){
	char *inputFilename = readString(stdin);
	FILE *inputFile = fopen(inputFilename, "r");
	
	switch (checkExtension(inputFilename)){
		case COMPRESS_FLAG:
			compress(inputFile);
			break;
		case UNCOMPRESS_FLAG:
			uncompress(inputFile);
			break;
	}
	
	free(inputFilename);
	fclose(inputFile);

	return 0;
}
