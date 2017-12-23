#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <path.h>
#include <maps.h>
#include <stack.h>
#include <utils.h>

void stackToPath(PathList *list, Stack *s){
	// alocando um novo caminho na lista
	list->paths = (Path *) realloc(list->paths, sizeof(Path) * (list->size + 1));

	// preenchendo valores do caminho a partir da pilha
	list->paths[list->size].roomNum = s->top + 1;
	list->paths[list->size].distance = 0;

	// copiando vetor da pilha para o caminho
	list->paths[list->size].rooms = (int *) malloc(sizeof(int) * 
												list->paths[list->size].roomNum);
	memcpy(list->paths[list->size].rooms, s->data, sizeof(int) *
												list->paths[list->size].roomNum);

	// atualizando numero de caminhos na lista
	list->size++;
}

PathList *getPaths(ChamberList *rooms, int start){
	int current;
	int i;
	bool newToVisit;

	// alocando lista de caminhos
	PathList *list = (PathList *) calloc(1, sizeof(PathList *));

	// alocando vetor de aparicoes (para evitar repeticoes no caminho)
	char *isRepeat = (char *) calloc(rooms->size, sizeof(char));

	// alocando pilha para navegacao no labirinto
	Stack *toVisit = stackCreate(rooms->size);

	// alocando e inicializando pilha de caminho auxiliar
	Stack *auxPath = stackCreate(rooms->size);
	stackPush(auxPath, start);

	// alocando pilha de agentes (camaras que chamam outras)
	Stack *agents = stackCreate(rooms->size);

	// inicializando navegacao
	current = start;

	// enquanto todos os caminhos ainda nao foram feitos
	while (!stackEmpty(auxPath)){

		// atualizando vetor de aparicoes
		isRepeat[current] = true;

		// atualizando caminho auxiliar
		if (current != start) stackPush(auxPath, current);

		// zerando flag de novas camaras na pilha de navegacao
		newToVisit = false;

		// colocando todos as conexoes com caminhos na pilha de navegacao
		for (i = 0; i < rooms->chambers[current].portalNum; i++){

			// caso a camara ainda nao esteja no caminho atual
			if (!isRepeat[rooms->chambers[current].portals[i]]){

				// coloque-a na pilha de navegacao
				stackPush(toVisit, rooms->chambers[current].portals[i]);

				// coloque quem a chamou na pilha de agentes
				stackPush(agents, current);

				// atualiza o contador de novas camaras
				newToVisit = true;
			}
				
		}

		// caso a camara atual seja uma saida
		if (current != start && rooms->chambers[current].isExit){
			stackToPath(list, auxPath);
		}

		// caso nenhuma nova camara tenha ido a pilha de navegacao
		if (!newToVisit){
			// o caminho deve retroceder ate que o topo do caminho auxiliar
			// possa continuar a pilha de navegacao

			// caso o topo do caminho auxiliar nao seja o agente do topo
			// da pilha de navegacao
			while (!stackEmpty(auxPath) &&
				!(stackView(auxPath) == stackView(agents))){

				// retire essa camara do caminho auxiliar
				isRepeat[stackView(auxPath)] = false;
				stackPop(auxPath);
			}
		}

		// continue para a proxima sala da pilha de navegacao
		current = stackPop(toVisit);
		stackPop(agents);
	}

	// liberando estruturas auxiliares
	free(isRepeat);
	stackDestroy(toVisit);
	stackDestroy(auxPath);
	stackDestroy(agents);

	return list;	
}

void getDistances(ChamberList *rooms, PathList *answers){
	int i, j;

	// corra por cada um dos caminhos
	for (i = 0; i < answers->size; i++){

		// calcule a distancia euclidiana entre cada duas camaras
		// e some no total do caminho
		for (j = 1; j < answers->paths[i].roomNum; j++)
			answers->paths[i].distance += euclidianDistance(
				rooms->chambers[answers->paths[i].rooms[j]].x,
				rooms->chambers[answers->paths[i].rooms[j]].y,
				rooms->chambers[answers->paths[i].rooms[j-1]].x,
				rooms->chambers[answers->paths[i].rooms[j-1]].y);
	}
}

bool pathCompare(Path *a, Path *b){
	int i;
	// caso as distancias sejam iguais, teste pela proxima
	if (a->distance == b->distance){
	//	printf("dist igual = %d\n", (int) a->distance);
		// caso o numero de camaras seja igual, teste pela proxima
		if (a->roomNum == b->roomNum){
		//	printf("quant igual = %d\n", a->roomNum);
			// teste a cada numero de camara
			for (i = 0; i < a->roomNum; i++){
			//	printf("checking [%d] and [%d]\n", a->rooms[i]+1, b->rooms[i]+1);
				if (a->rooms[i] > b->rooms[i]) return true;
				if (a->rooms[i] < b->rooms[i]) break;
			}
			//printf("nao troca por chamber\n");
			return false;
		} else {
			// caso diferentes, seja esse o criterio
			if (a->roomNum > b->roomNum) return true;
				else return false;
		}
	} else {
		// caso diferentes, seja esse o criterio
		if (a->distance > b->distance) return true;
			else return false;
	}
}

void pathSort(PathList *list){
	int i, j;
	Path aux;

	for (i = 0; i < list->size; i++){
		for (j = 0; j < list->size-i-1; j++){
			if (pathCompare((list->paths)+j, (list->paths)+j+1)){
				aux = list->paths[j];
				list->paths[j] = list->paths[j+1];
				list->paths[j+1] = aux;
			}
		}
	} 
}

void pathPrint(PathList *answers, int start){
	int i, j;
	printf("1 %d 0\n", start);
	for (i = 0; i < answers->size; i++){
		printf("%d ", answers->paths[i].roomNum);
		for (j = 0; j < answers->paths[i].roomNum; j++)
			printf("%d ", answers->paths[i].rooms[j]+1);
		printf("%d\n", (int) answers->paths[i].distance);
	}
}

void freePathList(PathList *list){
	int i;

	for (i = 0; i < list->size; i++)
		free(list->paths[i].rooms);
	free(list->paths);
	free(list);
}