#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <knn.h>

Mask *readMasks(int n, int size){
	/*  le n mascaras de convolucao a partir da entrada padrao */
	Mask *masks = (Mask *) malloc(sizeof(Mask)*n);
	int i;

	for (i = 0; i < n; i++) masks[i] = readDoubleMatrix(stdin, size, size);

	return masks;
}

void freeMasks(Mask *masks, int n, int size){
	/*  libera n mascaras de convolucao que estejam alocadas na
		memoria heap */
	n--;
	while (n >= 0) freeMatrix((void **) masks[n--], size);
	free(masks);
}

int fillFeatureVector(PGM *image, FeatureVector vec, int pos){
	/*  preenche uma parte de um vetor de caracteristicas
		com as informacoes sobre uma imagem convolucionada */	
	int i, j;
	double aux;

	for (i = 0; i < image->height; i++){
		// correndo por toda a linha
		for (j = 0; j < image->width; j++){
			// vendo a relacao do pixel com 0
			if (image->pixels[i][j] > 0) vec[pos+GREATER0]++;
			else if (image->pixels[i][j] == 0) vec[pos+EQUAL0]++;
			else vec[pos+LESS0]++;

			// somando media e entropia
			vec[pos+AVERAGE] += image->pixels[i][j];
			aux = log2(abslf(image->pixels[i][j]) + 1);
			vec[pos+ENTROPY] += (image->pixels[i][j]) * aux;
		}

		// calculando media e entropia
		vec[pos+AVERAGE] /= (image->width * 1.0);
		vec[pos+ENTROPY] *= -1.0;

		// correndo por toda a linha para somar variancia
		for (j = 0; j < image->width; j++){
			vec[pos+VARIANCE] += pow(image->pixels[i][j] - vec[pos+AVERAGE], 2);
		}

		// calculando variancia
		vec[pos+VARIANCE] /= (image->width * 1.0);

		// atualizando posicao no feature vector
		pos += NFEATURES;
	}

	return pos;
}

FeatureVector makeFeatureVector(PGM *image, Mask *masks, int maskNum, int maskSize){
	/*  gera um vetor de caracteristicas a partir de uma imagem base e
		uma serie de matrizes de convolucao */	
	PGM *aux;
	FeatureVector vector = (FeatureVector) 
					calloc(NFEATURES * image->height * maskNum, sizeof(double));
	int i;
	int pos = 0;

	for (i = 0; i < maskNum; i++){
		aux = PGMmasking(image, masks[i], maskSize);
		pos = fillFeatureVector(aux, vector, pos);
		PGMfree(aux);
	}

	return vector;
}

double getClass(FeatureVector test, FeatureVector *trainVec, PGM **train, int trainNum, int K, int vecSize){
	/*  retorna a classe de um vetor de caracteristicas a partir de uma serie de vetores
		de treinamento */	
	int i, j, l;
	Neighbor actualNeigh, auxNeigh; 

	Neighbor *neighbors = (Neighbor *) malloc(sizeof(Neighbor) * K);
	for (i = 0; i < K; i++) neighbors[i].dist = DBL_MAX;

	// corre por todos os vetores de treinamento
	for (i = 0; i < trainNum; i++){
		// calcula a distancia ate o vetor de teste
		actualNeigh.dist = euclidianDistance(test, trainVec[i], vecSize);
		actualNeigh.class = train[i]->class;

		// verifica se o vetor de teste esta entre os com K menores distancias
		for (j = 0; j < K; j++){
			if (actualNeigh.dist <= neighbors[j].dist){
				auxNeigh = neighbors[j];
				neighbors[j] = actualNeigh;
				break;
			}
		}

		// retorna o vetor de vizinhos a uma forma estavel
		if (j != K){ // houve trocas
			for (l = j+1; l < K; l++){
				actualNeigh = auxNeigh;
				auxNeigh = neighbors[l];
				neighbors[l] = actualNeigh;
			}
		}
	}

	ImageClass *chosenClasses = NULL;
	ImageClass key;
	int chosenCount = 0;
	double returnClass;

	// corre os K vizinhos mais proximos
	for (i = 0; i < K; i++){
		// caso uma classe ainda nao tenha sido encontrada
		for (j = 0; j < chosenCount; j++){
			if (chosenClasses[j].class == neighbors[i].class){
				chosenClasses[j].counter++;
				break;
			}
		}

		if (j == chosenCount){ // nao encontrou a classe
			chosenClasses = (ImageClass *) realloc(chosenClasses, sizeof(ImageClass) * (chosenCount + 1));
			chosenClasses[chosenCount].class = neighbors[i].class;
			chosenClasses[chosenCount].counter = 1;
			chosenCount++;
		}
	}

	// ordena o vetor de classes pela frequencia delas nos K vizinhos mais proximos
	for (i = 1; i < chosenCount; i++){
		key = chosenClasses[i];
		j = i-1;
		
		while (j >= 0 && key.counter > chosenClasses[j].counter){
			chosenClasses[j+1] = chosenClasses[j];
			j--;
		}
		
		chosenClasses[j+1] = key;
	}

	// retorna a classe mais encontrada
	returnClass = chosenClasses[0].class;

	free(neighbors);
	free(chosenClasses);

	return returnClass;
}