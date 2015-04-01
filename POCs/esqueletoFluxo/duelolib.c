#include "duelolib.h"

/* Copia uma matriz de rgb para outra */
void copia_matriz(camera *cam, unsigned char ***matriz_01, unsigned char ***matriz_02) {
	for (int y = 0; y < cam->altura; y++) {
        for (int x = 0; x < cam->largura; x++) {
            matriz_02[y][x][0] = matriz_01[y][x][0];
            matriz_02[y][x][1] = matriz_01[y][x][1];
            matriz_02[y][x][2] = matriz_01[y][x][2];
        }
    }
}

/* Compara frames para detectar se houve movimento */
bool compara_matriz(camera *cam, unsigned char ***matriz_original, unsigned char ***matriz, int range, int sensibilidade) {	
	int diferenca = 0;

	for(int y = 0; y < cam->altura; y++) {
	  
		for(int x = 0; x < cam->largura; x++) {
			int r = cam->quadro[y][x][0];
			int g = cam->quadro[y][x][1];
			int b = cam->quadro[y][x][2];  
	   
		    if(r >= matriz_original[y][x][0] - range && r <= matriz_original[y][x][0] + range &&
		       g >= matriz_original[y][x][1] - range && g <= matriz_original[y][x][1] + range &&
		       b >= matriz_original[y][x][2] - range && b <= matriz_original[y][x][2] + range) {
	        	matriz[y][x][0] = 0;
			    matriz[y][x][1] = 0;
			    matriz[y][x][2] = 0;
		    } else {
			    matriz[y][x][0] = 255;
			    matriz[y][x][1] = 255;
			    matriz[y][x][2] = 255;
	        	diferenca++;
		    }
		}		
	}
	if (diferenca > (cam->altura * cam->largura) / sensibilidade) {
		return true;
	} 

	return false;
}