#ifndef _MAPS_H_
#define _MAPS_H_

#include <utils.h>

typedef struct chamber {
	float x;
	float y;
	bool isExit;
	bool isExplicit;
	int *portals;
	int portalNum;
} Chamber;

typedef struct chamberlist {
	Chamber *chambers;
	int size;
} ChamberList;

ChamberList *readChambers();
void getPortals(ChamberList *list);
void freeChamberList(ChamberList *list);

#endif