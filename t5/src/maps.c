#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <maps.h>
#include <utils.h>

ChamberList *readChambers(){
	int i, explicitChambers, explicitId, exitInfo;

	// alocando lista de camaras
	ChamberList *list = (ChamberList *) malloc(sizeof(ChamberList));

	// lendo quantidade de pontos
	scanf("%d", &list->size);

	// alocando os pontos
	list->chambers = (Chamber *) calloc(list->size, sizeof(Chamber));

	// lendo os pontos
	for (i = 0; i < list->size; i++){
		scanf("%f%f", &list->chambers[i].x, &list->chambers[i].y);
	}

	// lendo quantidade de camaras explicitas
	scanf("%d", &explicitChambers);

	// lendo dados das camaras explicitas
	for (i = 0; i < explicitChambers; i++){
		scanf("%d%d", &explicitId, &exitInfo);

		// marcando que esse ponto eh uma camara explicita
		list->chambers[explicitId-1].isExplicit = true;

		// marque caso essa camara seja saida do labirinto
		if (exitInfo) list->chambers[explicitId-1].isExit = true;
	}

	return list;
}

void getPortals(ChamberList *list){
	int i, segmentNum, p1, p2, absp1, absp2;

	// lendo quantidade de segmentos
	scanf("%d", &segmentNum);

	// lendo dados dos segmentos
	for (i = 0; i < segmentNum; i++){
		scanf("%d%d", &p1, &p2);
		absp1 = abs(p1)-1;
		absp2 = abs(p2)-1;

		// verificando se o portal esta aberto
		// para estar fechado, um dos pontos deve ser uma camara explicita
		// e a sua entrada deve ser menor que zero
		if (!((list->chambers[absp1].isExplicit && p1 < 0) || 
			(list->chambers[absp2].isExplicit && p2 < 0))){

			// conectando duas camaras com um segmento
			// camara 1
			list->chambers[absp1].portals = (int *)
			realloc(list->chambers[absp1].portals, sizeof(int) * (list->chambers[absp1].portalNum + 1));
			list->chambers[absp1].portals[list->chambers[absp1].portalNum] = absp2;
			list->chambers[absp1].portalNum++;

			// camara 2
			list->chambers[absp2].portals = (int *)
			realloc(list->chambers[absp2].portals, sizeof(int) * (list->chambers[absp2].portalNum + 1));
			list->chambers[absp2].portals[list->chambers[absp2].portalNum] = absp1;
			list->chambers[absp2].portalNum++;

		}
	}
}

void freeChamberList(ChamberList *list){
	int i;

	for (i = 0; i < list->size; i++)
		free(list->chambers[i].portals);
	free(list->chambers);
	free(list);
}