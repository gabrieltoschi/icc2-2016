#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>

/*  [Tipo] bool: valores booleanos (verdadeiro e falso) */
typedef enum {
	false,
	true
} bool;

/*  [Tipo] Feature: facilita o acesso as informacoes de
	um vetor de caracteristicas */
typedef enum {
	GREATER0,
	EQUAL0,
	LESS0,
	AVERAGE,
	VARIANCE,
	ENTROPY,
	NFEATURES
} Feature;

/*  [Tipo] Mask: mascara de convolucao */
typedef double ** Mask;

/*  [Tipo] FeatureVector: vetor de caracteristicas */
typedef double * FeatureVector;

/*  [Tipo] Neighbor: guarda informacoes sobre um vizinho
	mais proximo, no algoritmo de KNN */
typedef struct {
	double dist;
	double class;
} Neighbor;

/*  [Tipo] ImageClass: guarda informacoes sobre uma classe
	de imagens, no algoritmo de KNN */
typedef struct {
	double class;
	int counter;
} ImageClass;

char *readString(FILE *stream, char delim);
int **allocIntMatrix(int width, int height);
int **readBinaryIntMatrix(FILE *stream, int width, int height);
void freeMatrix(void **m, int height);
double **readDoubleMatrix(FILE *stream, int width, int height);
char *copyString(char *string);
double abslf(double lf);
double euclidianDistance(double *a, double *b, int dimension);

#endif