#ifndef _KNN_H_
#define _KNN_H_

#include <pgm.h>
#include <utils.h>

Mask *readMasks(int n, int size);
void freeMasks(Mask *masks, int n, int size);
int fillFeatureVector(PGM *image, FeatureVector vec, int pos);
FeatureVector makeFeatureVector(PGM *image, Mask *masks, int maskNum, int maskSize);
double getClass(FeatureVector test, FeatureVector *trainVec, PGM **train, int trainNum, int K, int vecSize);

#endif