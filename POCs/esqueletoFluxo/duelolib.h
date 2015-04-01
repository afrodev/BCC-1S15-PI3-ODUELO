#ifndef __DUELOLIB_H__
#define __DUELOLIB_H__

#include "camera.h"

void copia_matriz(camera *, unsigned char ***, unsigned char ***);
bool compara_matriz(camera *, unsigned char ***, unsigned char ***, int, int);

#endif