/**
	>>>> FOURIER <<<<
	Funcoes que desempenham a transformada
	direta e inversa de Fourier e imprimem
	dados relativos ao processo.
**/

#ifndef _FOURIER_H_
#define _FOURIER_H_

#include <stdlib.h>
#include <complexnum.h>
#include <math.h>
#include <utils.h>

#define PI 4*atan(1.0)
#define RELEVANT_MIN 0.1

Complex *fourierCoeff(unsigned char *, int);
double *fourierMagnitude(Complex *, int);
int *fourierOrdenation(double *, int);
void fourierFilter(int *, Complex *, int, int);
unsigned char *fourierInverse(Complex *, int);
int getRelevant(double *, int);
void printFilterMagnitudes(int *, double *, int);
void printFilteredData(unsigned char *, int);

#endif
