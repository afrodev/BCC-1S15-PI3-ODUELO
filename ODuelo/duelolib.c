#include "duelolib.h"
#include <stdio.h>

void rgb_hsv(camera *cam, int **matiz, int **iluminacao) {
	for(int i = 0; i < cam->altura; i++){
		for(int j = 0; j < cam->largura; j++){
			float r = (float) cam->quadro[i][j][0] / 255;
			float g = (float) cam->quadro[i][j][1] / 255;
			float b = (float) cam->quadro[i][j][2] / 255;
			float aux;

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

			if (r == max)
				aux = ( g - b ) / delta;		// between yellow & magenta
			else if (g == max)
				aux = 2 + ( b - r ) / delta;	// between cyan & yellow
			else
				aux = 4 + ( r - g ) / delta;	// between magenta & cyan

			matiz[i][j] = aux * 60;				// degrees	
			if(matiz[i][j] < 0)
				matiz[i][j] += 360;

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
bool compara_matriz(camera *cam, unsigned char ***matriz_original, unsigned char ***matriz, int range, int sensibilidade, int sensibilidadeCor, bool *corPlayer, int *quantidadeAnterior, int qual) {	
	int diferenca = 0;
	int quantidade = 0;
	int corInicio = 47;
	int corFim = 68;

	int **matiz = malloc(cam->altura*sizeof(int *));
	int **iluminacao = malloc(cam->altura*sizeof(int *));
	for(int i=0; i < cam->altura;i++){
		matiz[i] = malloc(cam->largura*sizeof(int));
		iluminacao[i] = malloc(cam->largura*sizeof(int));
	}

	rgb_hsv(cam, matiz, iluminacao);

	for(int y = 0; y < cam->altura; y++) {
	  
		for(int x = 0; x < cam->largura; x++) {
			int r = cam->quadro[y][x][0] / 255;
			int g = cam->quadro[y][x][1] / 255;
			int b = cam->quadro[y][x][2] / 255;
			int cor = matiz[y][x];
            // int humberto = iluminacao[y][x]; 
	   
		    if (r <= matriz_original[y][x][0] - range || r >= matriz_original[y][x][0] + range ||
		       g <= matriz_original[y][x][1] - range || g >= matriz_original[y][x][1] + range ||
		       b <= matriz_original[y][x][2] - range || b >= matriz_original[y][x][2] + range) {
	        	diferenca++;
		    }
		    if (cor >= corInicio && cor <= corFim && r + g >= 1.5 && b <= 0.55){
		        matriz[y][x][0] = 255;
	            matriz[y][x][1] = 255;
	            matriz[y][x][2] = 255;
		        quantidade++;
	        } else {
	        	matriz[y][x][0] = 0;	
	            matriz[y][x][1] = 0;
	            matriz[y][x][2] = 0;
	        }
		}		
	}

	// printf("%d\n", quantidade - *quantidadeAnterior);
	// if (quantidade - *quantidadeAnterior > 5000) {
	// 	printf("atirou %d\n", qual);
	// 	// *corPlayer = true;
	// }

	for(int i = 0; i < cam->altura; i++){
		free(matiz[i]);
		free(iluminacao[i]);
	}
	free(matiz);
	free(iluminacao);

	*quantidadeAnterior = quantidade;

	if (diferenca > (cam->altura * cam->largura) / sensibilidade) {
		return true;
	}

	return false;
}