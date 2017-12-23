#include <pgm.h>

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <utils.h>

void PGMcomment(FILE *stream, char token, char end){
	/*  checa a existencia de comentario em uma imagem PGM e ignora-o na leitura,
		alterando a posição do cursor */
	char test = fgetc(stream);
	if (test == token) while (test != end) test = fgetc(stream);
		else ungetc(test, stream);
}

PGM *PGMread (char *filename){
	/*  le os dados de um arquivo PGM e o armazena em uma estrutura de dados
		do tipo PGM, a partir do nome do arquivo */
	PGM *image = (PGM *) malloc(sizeof(PGM));
	FILE *fp = fopen(filename, "r");

	image->magicNumber = readString(fp, '\n');
	PGMcomment(fp, '#', '\n');
	fscanf(fp, "%d", &image->width);
	fscanf(fp, "%d", &image->height);
	fscanf(fp, "%d", &image->maxValue);
	fgetc(fp);
	image->pixels = readBinaryIntMatrix(fp, image->width, image->height);
	image->class = -1.0;

	fclose(fp);
	return image;
}

void PGMfree(PGM *image){
	/*  libera os dados armazenados em memoria Heap de uma estrutura de arquivo PGM */
	free(image->magicNumber);
	freeMatrix((void *) image->pixels, image->height);
	free(image);
}

int isInside(int row, int col, int width, int height){
	/*  verifica se uma coordenada está dentro dos limites de uma matriz, 
		a partir do seu tamanho */
		if (row < 0 || col < 0 || row >= height || col >= width) return false;
			else return true;
}

PGM *PGMcreate(PGM *model){
	/*  cria uma nova imagem PGM em branco a partir dos atributos básicos
		de uma outra imagem PGM */
	PGM *new = (PGM *) malloc(sizeof(PGM));
	new->magicNumber = copyString(model->magicNumber);
	new->width = model->width;
	new->height = model->height;
	new->maxValue = model->maxValue;
	new->class = model->class;
	new->pixels = allocIntMatrix(model->width, model->height);

	return new;
}

PGM *PGMmasking(PGM *image, Mask mask, int maskSize){
	/*  aplica uma máscara de numeros reais em uma imagem PGM, a partir de uma operacao
		de convolucao de matrizes */
	PGM *result;
	int i, j, k, l;
	int aux;
	int conversion = maskSize / 2;

	// preparacao da nova imagem PGM
	result = PGMcreate(image);

	for (i = 0; i < image->height; i++){
		for (j = 0; j < image->width; j++){
			aux = 0;
			// laços de convolucao começam em numeros negativos para representar uma
			// 											"expansao virtual" da matriz
			for (k = conversion*-1; k <= conversion; k++){
				for (l = conversion*-1; l <= conversion; l++){
					// checa se o pixel a ser checado esta dentro dos limites da matriz
					if (isInside(k+i, j+l, image->width, image->height)){
						aux += (image->pixels[k+i][j+l] *
								mask[k+conversion][l+conversion]);
					}
				}
			}
			result->pixels[i][j] = aux;
		}
	}
	return result;
}

PGM **PGMmassiveRead(char *massiveFilename, char *classFilename, int *n){
	/*  le varias imagens PGM a partir de um arquivo que contem o
		nome do arquivo onde cada uma esta inserida */	
	FILE *imageFile = fopen(massiveFilename, "r");
	FILE *classFile = NULL;
	if (classFilename != NULL) classFile = fopen(classFilename, "r");
	char *imageFilename;
	PGM **images = NULL;
	*n = 0;

	do {
		imageFilename = readString(imageFile, '\n');
		if (!feof(imageFile)){
			images = (PGM **) realloc(images, sizeof(PGM *)*(*n + 1));
			images[*n] = PGMread(imageFilename);
			if (classFilename != NULL) fscanf(classFile, "%lf", &images[*n]->class);
			(*n)++;
		}
		if (imageFilename != NULL) free(imageFilename);
	} while (!feof(imageFile));

	fclose(imageFile);
	if (classFile != NULL) fclose(classFile);
	return images;
}

void PGMmassiveFree(PGM **images, int n){
	/*  libera n imagens PGM que estejam alocadas na
		memoria heap */
	n--;
	while (n >= 0) PGMfree(images[n--]);
	free(images);
}