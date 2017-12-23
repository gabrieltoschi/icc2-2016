#ifndef _PGM_H_
#define _PGM_H_

#include <utils.h>

/*  [Tipo] PGM: armazena todas as informacoes sobre uma imagem PGM */
typedef struct {
	char *magicNumber;
	int height;
	int width;
	int maxValue;
	int **pixels;
	double class;
} PGM;

PGM *PGMread (char *filename);
void PGMfree(PGM *image);
PGM *PGMmasking(PGM *image, Mask mask, int maskSize);
PGM **PGMmassiveRead(char *massiveFilename, char *classFilename, int *n);
void PGMmassiveFree(PGM **images, int n);

#endif