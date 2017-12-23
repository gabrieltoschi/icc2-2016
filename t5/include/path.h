#ifndef _PATH_H_
#define _PATH_H_

#include <maps.h>

typedef struct path {
	int roomNum;
	int *rooms;
	float distance;
} Path;

typedef struct pathlist {
	Path *paths;
	int size;
} PathList;

PathList *getPaths(ChamberList *rooms, int start);
void getDistances(ChamberList *rooms, PathList *answers);
void pathSort(PathList *list);
void pathPrint(PathList *answers, int start);
void freePathList(PathList *list);

#endif