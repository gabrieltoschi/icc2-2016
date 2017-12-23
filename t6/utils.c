#include <utils.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* funcoes para ordenacao de numeros inteiros usando
	o algoritmo quicksort */
void swapInt(int *v, int a, int b){
	int aux = v[a];
	v[a] = v[b];
	v[b] = aux;
}

int indexPartitionInt(int *v, int *freq, int l, int r){
	int i, j;
	
	i = l;
	for (j = i+1; j <= r; j++){
		if (freq[v[j]] > freq[v[l]]){
			++i;
			swapInt(v, i, j);
		}
	}
	swapInt(v, i, l);
	
	return i;
}

void indexQuicksortInt(int *v, int *freq, int l, int r){
	int c;
	
	if (l < r){
		c = indexPartitionInt(v, freq, l, r);
		indexQuicksortInt(v, freq, l, c-1);
		indexQuicksortInt(v, freq, c+1, r);
	}
}

/* ordena um vetor de indices a partir de um vetor de frequencias */
int *sortByFreq(int *freq, int size){
	// criando vetores de indices para ordenacao
	int *index, *access;
	int i;
	
	index = (int *) malloc(sizeof(int) * size);
	for (i = 0; i < size; i++) index[i] = i;
	
	// chamando algoritmo quicksort para ordenacao
	indexQuicksortInt(index, freq, 0, size-1);

	// melhorando o acesso aos valores ordenados
	access = (int *) malloc(sizeof(int) * size);
	for (i = 0; i < size; i++) access[index[i]] = i;
	
	free(index);
	return access;
}