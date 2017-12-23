/**
	>>>> UTILS <<<<
	Funcoes genericas para serem usadas em
	todo o programa, como leitura e ordenacao.
**/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>

unsigned char *readBinaryFile(FILE *, int *);
void indexInsertionSort(int *, double *, int);

#endif
