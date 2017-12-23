/**
	>>>> COMPLEXNUM <<<<
	Funcoes para manipulacao de numeros
	complexos.
**/

#ifndef _COMPLEXNUM_H_
#define _COMPLEXNUM_H_

typedef struct {
	double real;
	double imag;
} Complex;

Complex complexInit(double, double);
Complex complexSum(Complex, Complex);
Complex complexMult(Complex, Complex);
Complex complexMultScal(Complex, double);
double complexGetReal(Complex);
double complexGetImag(Complex);

#endif
