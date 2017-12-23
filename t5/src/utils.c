#include <utils.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

float euclidianDistance(float xA, float yA, float xB, float yB){
	/*  calcula a distancia euclidiana entre dois vetores no R2 */
	float dist = 0.0;
	dist += pow(xA - xB, 2);
	dist += pow(yA - yB, 2);
	return sqrt(dist);
}
