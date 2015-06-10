#ifndef __DUELOLIB_H__
#define __DUELOLIB_H__

#include "camera.h"

typedef struct{
	float h;
	float s;
	float v;
}hsv;

void rgb_hsv(camera *, int **, int **);
void copia_matriz(camera *, unsigned char ***, unsigned char ***);
bool compara_matriz(camera *, unsigned char ***, unsigned char ***, int, int, bool *, int *, int, int, int, float, float);

#endif