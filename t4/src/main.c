#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <pgm.h>
#include <utils.h>
#include <knn.h>

int main(int argc, char *argv[]){

	// leitura dos nomes de arquivos
	char *trainFilename, *trainClassFilename, *testFilename;
	scanf("%ms", &trainFilename);
	scanf("%ms", &trainClassFilename);
	scanf("%ms", &testFilename);
	int maskNum, maskSize, K, i;

	// leitura de mascaras e dados para KNN
	scanf("%d%d", &maskNum, &maskSize);
	Mask *masks = readMasks(maskNum, maskSize);
	scanf("%d", &K);

	// leitura de PGMs
	int trainNum, testNum;
	PGM **train = PGMmassiveRead(trainFilename, trainClassFilename, &trainNum);
	PGM **test = PGMmassiveRead(testFilename, NULL, &testNum);

	// convolucao/geracao de feature vectors de treinamento
	int vectorSize = NFEATURES * train[0]->height * maskNum;
	FeatureVector *trainVectors = (FeatureVector *) malloc(sizeof(FeatureVector) * trainNum);
	for (i = 0; i < trainNum; i++){
		trainVectors[i] = makeFeatureVector(train[i], masks, maskNum, maskSize);
	}

	// recebendo classe das imagens de teste
	FeatureVector testVector;
	for (i = 0; i < testNum; i++){
		testVector = makeFeatureVector(test[i], masks, maskNum, maskSize);
		PGMfree(test[i]);
		printf("%.3lf\n", getClass(testVector, trainVectors, train, trainNum, K, vectorSize));
		free(testVector);
	}
	free(test);

	// liberacao de memoria heap
	freeMatrix((void **) trainVectors, trainNum);
	free(trainFilename);
	free(trainClassFilename);
	free(testFilename);
	freeMasks(masks, maskNum, maskSize);
	PGMmassiveFree(train, trainNum);

	return 0;
}