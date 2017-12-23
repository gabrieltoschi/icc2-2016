/**
	>>>> UTILS <<<<
	Funcoes genericas para serem usadas em
	todo o programa, como leitura e ordenacao.
**/

#include <utils.h>

/** >> readBinaryFile
	Le um arquivo binario e armazena os valores
	em um vetor de unsigned char.
**/
unsigned char *readBinaryFile(FILE *stream, int *ct){
  	unsigned char *binary;
	
	fseek(stream, 0, SEEK_END);
	*ct = ftell(stream);
	rewind(stream);
	
	binary = (unsigned char *) malloc(*ct);
	fread(binary, sizeof(unsigned char), *ct, stream);
	
	return binary;
}

/** >> indexInsertionSort
	Usa o algoritmo de insertion sort para ordenar um vetor
	de indices, usando outro vetor de valores como referencia.
**/
void indexInsertionSort(int *index, double *value, int n){
	int i, j = 0, key;
	
	for (i = 1; i < n; i++){
		key = index[i];
		j = i-1;
		
		while (j >= 0 && value[key] > value[index[j]]){
			index[j+1] = index[j];
			j--;
		}
		
		index[j+1] = key;
	}
} 
