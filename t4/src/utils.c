#include <utils.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

char *readString(FILE *stream, char delim){
	/*  lê uma string ate um delimitador e armazena na memoria Heap */
	char *str = NULL;
	char c;
	int ct = 0;

	do {
		c = fgetc(stream);
		str = (char *) realloc(str, sizeof(char)*(ct+1));
		str[ct++] = c;
	} while (c != delim && c != EOF);

	str[--ct] = '\0';
	return str;
}

int **allocIntMatrix(int width, int height){
	/*  aloca uma matriz de inteiros na memoria Heap */
	int i;
	int **m = (int **) malloc(sizeof(int *)*height);

	for (i = 0; i < height; i++)
		m[i] = (int *) calloc(width, sizeof(int));

	return m;
}

int **readBinaryIntMatrix(FILE *stream, int width, int height){
	/*  le, a partir de um arquivo binario, valores inteiros e os armazena em uma matriz */
	int **m = allocIntMatrix(width, height);
	unsigned char aux;
	int i, j;
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			fread(&aux, sizeof(unsigned char), 1, stream);
			m[i][j] = aux;
		}
	}

	return m;
}

void freeMatrix(void **m, int height){
	/*  libera uma matriz alocada na memoria Heap */
	int i;
	for (i = 0; i < height; i++) free(m[i]);
	free(m);
}

double **allocDoubleMatrix(int width, int height){
	/*  aloca uma matriz de números reais na memoria Heap */
	int i;
	double **m = (double **) malloc(sizeof(double *)*height);

	for (i = 0; i < height; i++)
		m[i] = (double *) malloc(sizeof(double)*width);

	return m;
}

double **readDoubleMatrix(FILE *stream, int width, int height){
	/*  le, a partir de um arquivo, valores reais e os armazena em uma matriz */
	double **m = allocDoubleMatrix(width, height);
	int i, j;
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			fscanf(stream, "%lf", &m[i][j]);
		}
	}

	return m;
}

char *copyString(char *string){
	/*  copia uma string para outra região da memória Heap, já alocando
		a memória necessária */
	char *new = (char *) malloc(sizeof(char)*(strlen(string)+1));
	strcpy(new, string);
	return new;
}

double abslf(double lf){
	/*  retorna o valor absoluto de um numero real */
	return lf < 0 ? (lf * -1.0) : lf;
}

double euclidianDistance(double *a, double *b, int dimension){
	/*  calcula a distancia euclidiana entre dois vetores,
		de dimensao n */
	double dist = 0.0;

	while (dimension >= 0){
		dist += pow(a[dimension] - b[dimension], 2);
		dimension--;
	}

	return sqrt(dist);
}
