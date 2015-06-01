#include "duelolib.h"
#include <stdio.h>

void rgb_hsv(camera *cam, int **matiz, int **iluminacao){
	for(int i = 0; i < cam->altura; i++){
		for(int j = 0; j < cam->largura; j++){
			float r = (float) cam->quadro[i][j][0] / 255;
			float g = (float) cam->quadro[i][j][1] / 255;
			float b = (float) cam->quadro[i][j][2] / 255;

			float max, min, delta;

			if(r > b)
				max = r;
			else
				max = b;

			if(g > max)
				max = g;

			if(r < b)
				min = r;
			else
				min = b;

			if(g < min)
				min = g;

			delta = max - min;

			if(max == r){
				if(g >= b){
					matiz[i][j] = 60*((g - b)/delta);
				}
				else{
					matiz[i][j] = 360 + 60*((g - b)/delta);
				}
			}
			else if(max == g){
				matiz[i][j] = 120 + 60*((b - r)/delta);
			}
			else{
				matiz[i][j] = 240 + 60*((r - g)/delta);
			}

			iluminacao[i][j] = max*100;

		}
	}
}

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
bool compara_matriz(camera *cam, unsigned char ***matriz_original, unsigned char ***matriz, int range, int sensibilidade, int sensibilidadeCor, bool *corPlayer) {	
	int diferenca = 0;
	int quantidade = 0;

	int **matiz = malloc(cam->altura*sizeof(int *));
	int **iluminacao = malloc(cam->altura*sizeof(int *));
	for(int i=0; i < cam->altura;i++){
		matiz[i] = malloc(cam->largura*sizeof(int));
		iluminacao[i] = malloc(cam->largura*sizeof(int));
	}

	rgb_hsv(cam, matiz,iluminacao);

	for(int y = 0; y < cam->altura; y++) {
	  
		for(int x = 0; x < cam->largura; x++) {
			int r = cam->quadro[y][x][0];
			int g = cam->quadro[y][x][1];
			int b = cam->quadro[y][x][2];
			int mario = matiz[y][x];
            int humberto = iluminacao[y][x]; 
	   
		    if(r <= matriz_original[y][x][0] - range || r >= matriz_original[y][x][0] + range ||
		       g <= matriz_original[y][x][1] - range || g >= matriz_original[y][x][1] + range ||
		       b <= matriz_original[y][x][2] - range || b >= matriz_original[y][x][2] + range) {
	        	diferenca++;
		    }
		    if(mario >= 340 && humberto >= 70) {
            	quantidade++;
            }
		}		
	}

	if (quantidade > (cam->altura * cam->largura) / sensibilidadeCor) {
		*corPlayer = true;
	}
	if (diferenca > (cam->altura * cam->largura) / sensibilidade) {
		return true;
	}

	for(int i = 0; i < cam->altura; i++){
		free(matiz[i]);
		free(iluminacao[i]);
	}
	free(matiz);
	free(iluminacao);

	return false;
}