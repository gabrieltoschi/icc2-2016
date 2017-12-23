/**
	>>>> FOURIER <<<<
	Funcoes que desempenham a transformada
	direta e inversa de Fourier e imprimem
	dados relativos ao processo.
**/

#include <fourier.h>

/** >> fourierCoeff
	Gera os coeficientes da transformada direta
	de Fourier a partir de um vetor de dados.
**/
Complex *fourierCoeff(unsigned char *data, int n){
	Complex *coeff = (Complex *) calloc((n/2)+1, sizeof(Complex));
	Complex aux;
	double epsilon;
	int k, t;
	
	// aplicando a formula da transformada direta
	for (k = 0; k <= n/2; k++){
		for (t = 0; t < n; t++){
			aux = complexInit(cos(k * 2*PI * t / (n * 1.0)), -1.0 * sin(k * 2*PI * t / (n * 1.0)));
			aux = complexMultScal(aux, data[t]);
			coeff[k] = complexSum(coeff[k], aux);
		}
		epsilon = k == 0 ? 1 : 2;
		coeff[k] = complexMultScal(coeff[k], epsilon / (n * 1.0));
	}
	
	return coeff;
}

/** >> fourierMagnitude
	Gera as magnitudes a partir de um vetor
	de coeficientes complexos.
**/
double *fourierMagnitude(Complex *coeff, int n){
	double *mag = (double *) malloc(sizeof(double)*((n/2)+1));
	int i;
	
	for (i = 0; i < (n/2)+1; i++)
		mag[i] = sqrt(pow(complexGetReal(coeff[i]), 2) + pow(complexGetImag(coeff[i]), 2));
	
	return mag;
}

/** >> fourierOrdenation
	Gera um vetor de indices e ordena eles a partir das
	magnitudes. 
**/
int *fourierOrdenation(double *mag, int n){
	int i;
	// cria vetor de indices
	int *ordIndex = (int *) malloc(sizeof(int)*((n/2)+1));
	for (i = 0; i < (n/2)+1; i++) ordIndex[i] = i;
	
	// ordena vetor a partir das magnitudes
	indexInsertionSort(ordIndex, mag, (n/2)+1);
	return ordIndex;
}

/** >> fourierFilter
	Zera os coeficientes que não são relevantes a partir
	do vetor ordenado de indices.
**/
void fourierFilter(int *index, Complex *coeff, int counter, int n){
	int i;
	for (i = counter; i < (n/2)+1; i++)
		coeff[index[i]] = complexInit(0.0, 0.0);
}

/** >> fourierInverse
	Realiza a transformada inversa de Fourier.
**/
unsigned char *fourierInverse(Complex *coeff, int n){
	int t, k;
	Complex aux, sumAux;
	double realAux;
	unsigned char *inverse = (unsigned char *) calloc(n, sizeof(unsigned char));

	// aplicando a formula da transformada inversa
	for (t = 0; t < n; t++){
		sumAux = complexInit(0.0, 0.0);
		for (k = 0; k <= n/2; k++){
			aux = complexInit(cos(k * 2*PI * t / (n * 1.0)), sin(k * 2*PI * t / (n * 1.0)));
			aux = complexMult(aux, coeff[k]);
			sumAux = complexSum(sumAux, aux);
		}
		realAux = round(complexGetReal(sumAux));
		inverse[t] = (unsigned char) realAux;
	}

	return inverse;
}

/** >> getRelevant
	Retorna o numero de magnitudes maiores que um
	valor determinado RELEVANT_MIN.
**/
int getRelevant(double *mag, int n){
	int i, rel = 0;
	
	for (i = 0; i < (n/2)+1; i++)
		if (mag[i] > RELEVANT_MIN) rel++;
		
	return rel;
}

/** >> printFilterMagnitudes
	Imprime as magnitudes mais relevantes.
**/
void printFilterMagnitudes(int *index, double *mag, int counter){
	int i;
	for (i = 0; i < counter-1; i++)
		printf("%u ", (unsigned char) mag[index[i]]);
	printf("%u\n", (unsigned char) mag[index[counter-1]]);
}

/** >> printFilteredData
	Imprime um vetor de dados ja filtrados,
	apos passar pela transformada inversa
	de Fourier.
**/
void printFilteredData(unsigned char *data, int n){
	int i;
	for (i = 0; i < n; i++)
		printf("%d\n", data[i]);
}
