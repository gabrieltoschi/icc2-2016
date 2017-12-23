/**
	>>>> COMPLEXNUM <<<<
	Funcoes para manipulacao de numeros
	complexos.
**/

#include <complexnum.h>

/** >> complexInit
	Inicializa um numero complexo a partir
	de sua parte real e imaginaria.
**/
Complex complexInit(double real, double imag){
	Complex new;
	new.real = real;
	new.imag = imag;
	return new;
}

/** >> complexSum
	Soma dois numeros complexos.
**/
Complex complexSum(Complex a, Complex b){
	Complex sum;
	sum.real = a.real + b.real;
	sum.imag = a.imag + b.imag;
	return sum;
}

/** >> complexMult
	Multiplica dois numeros complexos.
**/
Complex complexMult(Complex a, Complex b){
	Complex mult;
	mult.real = (a.real*b.real) - (a.imag*b.imag);
	mult.imag = (a.real*b.imag) + (a.imag*b.real);
	return mult;
}

/** >> complexMultScal
	Multiplica um numero complexo por um numero real.
**/
Complex complexMultScal(Complex c, double scalar){
	Complex mult;
	mult.real = c.real * scalar;
	mult.imag = c.imag * scalar;
	return mult;
}

/** >> complexGetReal
	Retorna a parte real de um numero complexo.
**/
double complexGetReal(Complex c){
	return c.real;
}

/** >> complexGetImag
	Retorna a parte imaginaria de um numero complexo.
**/
double complexGetImag(Complex c){
	return c.imag;
}
