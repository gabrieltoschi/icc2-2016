#include <stdlib.h>
#include <stdio.h>

#include <maps.h>
#include <path.h>

int main(int argc, char *argv[]){
	int startChamber;
	
	// lendo camaras do labirinto
	ChamberList *rooms = readChambers();

	// lendo portais do labirinto
	getPortals(rooms);

	// lendo camara inicial
	scanf("%d", &startChamber);

	// calculando caminhos no labirinto
	PathList *answers = getPaths(rooms, startChamber-1);

	// calculando distancias dos caminhos
	getDistances(rooms, answers);

	// ordenando os caminhos
	pathSort(answers);

	// imprimindo os caminhos
	pathPrint(answers, startChamber);

	// liberando memoria alocada
	freeChamberList(rooms);
	freePathList(answers);

	return 0;
}