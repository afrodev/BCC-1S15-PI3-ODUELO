#include "duelolib.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

void rgb_hsv(camera *cam, int **matiz, int **saturacao) {
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

			saturacao[i][j] = delta / max;

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
bool compara_matriz(camera *cam, unsigned char ***matriz_original, unsigned char ***matriz, int range, int sensibilidade, bool *corPlayer, int *quantidadeAnterior, int qual, int corInicio, int corFim, float saturValor) {	
	int diferenca = 0;
	int quantidade = 0;

	// int **matiz = malloc(cam->altura*sizeof(int *));
	// int **saturacao = malloc(cam->altura*sizeof(int *));
	// for(int i=0; i < cam->altura;i++){
	// 	matiz[i] = malloc(cam->largura*sizeof(int));
	// 	saturacao[i] = malloc(cam->largura*sizeof(int));
	// }

	// rgb_hsv(cam, matiz, saturacao);

	for(int y = 0; y < cam->altura; y++) {
	  
		for(int x = 0; x < cam->largura; x++) {
			int r = cam->quadro[y][x][0];
			int r2 = r / 255;
			int g = cam->quadro[y][x][1];
			int g2 = g / 255;
			int b = cam->quadro[y][x][2];
			int b2 = b / 255;
			float cor;
            float saturacao;
            float value;
	   
            al_color_rgb_to_hsv(r, g, b, &cor, &saturacao, &value);

		    if (r <= matriz_original[y][x][0] - range || r >= matriz_original[y][x][0] + range ||
		       g <= matriz_original[y][x][1] - range || g >= matriz_original[y][x][1] + range ||
		       b <= matriz_original[y][x][2] - range || b >= matriz_original[y][x][2] + range) {
	        	diferenca++;
		    }
		    if (cor >= corInicio && cor <= corFim && r2 + g2 >= 1.5 && b2 <= 0.55 && saturacao >= saturValor){
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
	if (quantidade - *quantidadeAnterior > 5000) {
	 	// printf("atirou %d\n", qual);
		*corPlayer = true;
	}

	// for(int i = 0; i < cam->altura; i++){
	// 	free(matiz[i]);
	// 	free(saturacao[i]);
	// }
	// free(matiz);
	// free(saturacao);

	*quantidadeAnterior = quantidade;

	if (diferenca > (cam->altura * cam->largura) / sensibilidade) {
		return true;
	}

	return false;
}