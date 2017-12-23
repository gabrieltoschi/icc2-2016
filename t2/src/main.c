#include <stdlib.h>
#include <stdio.h>

#include <utils.h>
#include <fourier.h>
#include <complexnum.h>


int main(int argc, char *argv[]){
	
	char audioFileName[15];
	int filterCount;
	int observCount;
	unsigned char *audioData, *filteredData;
	Complex *coeff;
	double *magnitude;
	int *ordenated;
	
	// leitura do nome do arquivo e da quantidade de
	// coeficientes relevantes
	scanf("%s%d", audioFileName, &filterCount);
	FILE *audioFile = fopen(audioFileName, "r");
	
	// leitura dos dados binarios do arquivo
	audioData = readBinaryFile(audioFile, &observCount);
	
	// geracao de coeficientes
	coeff = fourierCoeff(audioData, observCount);
	
	// geracao de magnitudes
	magnitude = fourierMagnitude(coeff, observCount);
	
	// ordenacao de um vetor de indices a partir das magnitudes
	ordenated = fourierOrdenation(magnitude, observCount);
	
	// deixando apenas os coeficientes mais relevantes
	fourierFilter(ordenated, coeff, filterCount, observCount);
	
	// fazendo a transformada inversa de Fourier
	filteredData = fourierInverse(coeff, observCount);
	
	// imprimindo dados na saida
	printf("%d\n", observCount);
	printf("%d\n", getRelevant(magnitude, observCount));
	printFilterMagnitudes(ordenated, magnitude, filterCount);
	printFilteredData(filteredData, observCount);

	// liberacao de memoria heap
	fclose(audioFile);
	free(audioData);
	free(coeff);
	free(magnitude);
	free(ordenated);
	free(filteredData);
	return 0;
}
