/*
 * Projeto Integrador III
 * Bacharelado em Ciência da Computação - Centro Universitário Senac
 *
 * Professor Orientador: Marcelo Hashimoto
 *
 * Grupo:
 * - Mario Roberto Suruagu de Castro
 * - Humberto Vieira de Castro
 * - William Collecta de Alvelos
 *
 *
 * POC de detecção de movimento 
 * Compara o frame atual da camera com o frame anterior para detectar se houve movimento
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "camera.h"


typedef struct{
	float h;
	float s;
	float v;	

}hsv;

ALLEGRO_FONT *fonte = NULL;


// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
// void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
// {
// 	float min, max, delta;
// 	min = MIN( r, g, b );
// 	max = MAX( r, g, b );
// 	*v = max;				// v
// 	delta = max - min;
// 	if( max != 0 )
// 		*s = delta / max;		// s
// 	else {
// 		// r = g = b = 0		// s = 0, v is undefined
// 		*s = 0;
// 		*h = -1;
// 		return;
// 	}
// 	if( r == max )
// 		*h = ( g - b ) / delta;		// between yellow & magenta
// 	else if( g == max )
// 		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
// 	else
// 		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
// 	*h *= 60;				// degrees
// 	if( *h < 0 )
// 		*h += 360;
// }

// void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
// {
// 	int i;
// 	float f, p, q, t;
// 	if( s == 0 ) {
// 		// achromatic (grey)
// 		*r = *g = *b = v;
// 		return;
// 	}
// 	h /= 60;			// sector 0 to 5
// 	i = floor( h );
// 	f = h - i;			// factorial part of h
// 	p = v * ( 1 - s );
// 	q = v * ( 1 - s * f );
// 	t = v * ( 1 - s * ( 1 - f ) );
// 	switch( i ) {
// 		case 0:
// 			*r = v;
// 			*g = t;
// 			*b = p;
// 			break;
// 		case 1:
// 			*r = q;
// 			*g = v;
// 			*b = p;
// 			break;
// 		case 2:
// 			*r = p;
// 			*g = v;
// 			*b = t;
// 			break;
// 		case 3:
// 			*r = p;
// 			*g = q;
// 			*b = v;
// 			break;
// 		case 4:
// 			*r = t;
// 			*g = p;
// 			*b = v;
// 			break;
// 		default:		// case 5:
// 			*r = v;
// 			*g = p;
// 			*b = q;
// 			break;
// 	}
// }


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


//busca pela cor verde que seria o LED

bool busca_cor(camera *cam, int sensibilidade, unsigned char ***matriz){

	int quantidade = 0;



	//al_draw_textf(fonte, al_map_rgb(0, 0, 0), 30, 30, ALLEGRO_ALIGN_LEFT, "APONTE O LED VERDE");

	for(int y = 0; y < cam->altura; y++)
        for(int x = 0; x < cam->largura; x++) {
          float r = (cam->quadro[y][x][0]) / 255;
          float g = (cam->quadro[y][x][1]) / 255;
          float b = (cam->quadro[y][x][2]) / 255;

          	if(g < r) {
            //cy += y;
            //cx += x;
            //cn += 1;

            matriz[y][x][0] = 255;
            matriz[y][x][1] = 255;
            matriz[y][x][2] = 255;

            quantidade++;

          }
          else {
            matriz[y][x][0] = 0;	
            matriz[y][x][1] = 0;
            matriz[y][x][2] = 0;
          }
        }

        //printf("saio daqui caraio\n");
        //return false;

       	if(quantidade/sensibilidade > 1)
       		return true;

       	else
       		return false;

}




void jogo() {
	bool finalizado = false;
	bool renderizar = true;
    bool movimentoPlayer1 = false;
    bool movimentoPlayer2 = false;
    int sensibilidade = 100;
	int range = 70;

	int pontosPlayer1 = 0;
	int pontosPlayer2 = 0;

	const int FPS = 60;
        
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
	ALLEGRO_BITMAP *imagem_esq = NULL;
    ALLEGRO_BITMAP *imagem_dir = NULL;
    ALLEGRO_BITMAP *imagem_esq_baixo = NULL;
    ALLEGRO_BITMAP *imagem_dir_baixo = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *ganha = NULL;


	camera *cam = camera_inicializa(0);
	if (!cam) {
		fprintf(stderr, "Falha ao Inicializar a Camera\n");
		//return EXIT_FAILURE;
	}

	camera *cam2 = camera_inicializa(1);
	if (!cam2) {
		fprintf(stderr, "Falha ao Inicializar a Segunda Camera\n");
		//return EXIT_FAILURE;
	}

	// matriz para a primeira camera
	unsigned char ***matriz = camera_aloca_matriz(cam);
    unsigned char ***matriz_anterior = camera_aloca_matriz(cam);

    //para a segunda camera
	unsigned char ***matriz2 = camera_aloca_matriz(cam2);
    unsigned char ***matriz_anterior2 = camera_aloca_matriz(cam2);





	if (!al_init()) {
		fprintf(stderr, "Falha ao Carregar Allegro5\n");
		//return EXIT_FAILURE;
	}

	al_get_display_mode(0, &disp_data);

	int largCam = cam->largura;
	int altCam = cam->altura;
	float largImg = disp_data.width / 2;
	float altImg = (float) altCam / (largCam / largImg);

	const int LARG = largImg * 2;
	const int ALT = altImg;


	janela = al_create_display(LARG*2, ALT);
	if (!janela) {
		fprintf(stderr, "Falha ao Iniciar Janela\n");
		//return EXIT_FAILURE;
	}



	al_set_window_position(janela, (disp_data.width - LARG) / 2, (disp_data.height - ALT) / 2);
	al_set_window_title(janela, "O DUELO");

	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	fila_de_eventos = al_create_event_queue();
	if (!fila_de_eventos) {
		fprintf(stderr, "Falha ao Criar Fila de Eventos\n");
		//return EXIT_FAILURE;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Falha ao Criar Timer\n");
		//return EXIT_FAILURE;
	}

	fonte = al_load_font("georgiab.ttf", 24, 0);
	if (!fonte)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		//return EXIT_FAILURE;
	}

	imagem_esq = al_create_bitmap(largCam, altCam);
    imagem_dir = al_create_bitmap(largCam, altCam);
    ganha = al_create_bitmap(largCam, altCam);


	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

	al_start_timer(timer);
    camera_atualiza(cam);
    //camera_atualiza(cam2);
    copia_matriz(cam, cam->quadro, matriz_anterior);
    //copia_matriz(cam2, cam2->quadro, matriz_anterior);

	int parado = 0;
	while(!finalizado) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_de_eventos, &evento);

		if (evento.type == ALLEGRO_EVENT_TIMER && !renderizar) {
			renderizar = true;

			camera_atualiza(cam);
			camera_atualiza(cam2);
            
            movimentoPlayer1 = compara_matriz(cam, matriz_anterior, matriz, range, sensibilidade);
            movimentoPlayer2 = compara_matriz(cam2, matriz_anterior2, matriz2, range, sensibilidade);


            copia_matriz(cam, cam->quadro, matriz_anterior);
            copia_matriz(cam2, cam2->quadro, matriz_anterior2);

		} else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			finalizado = true;
		} else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					finalizado = true;
					break;
				case ALLEGRO_KEY_SPACE:
					break;
				case ALLEGRO_KEY_UP:
					range += 5;
					break;
				case ALLEGRO_KEY_DOWN:
					range -= 5;
					if (range < 0)
						range = 0;
					break;
                case ALLEGRO_KEY_RIGHT:
                    sensibilidade += 5;
                    break;
                case ALLEGRO_KEY_LEFT:
                    sensibilidade -= 5;
                    if (sensibilidade < 5)
                        sensibilidade = 5;
                    break;
			}
		}
		if (renderizar && al_is_event_queue_empty(fila_de_eventos)) {
			renderizar = false;

			camera_copia(cam, cam->quadro, imagem_esq);
            //camera_copia(cam, matriz, imagem_dir);

            //para duas cameras substituir linha de cima por linha comentada
            camera_copia(cam2, cam2->quadro, imagem_dir);

            al_draw_scaled_bitmap(imagem_esq, 0, 0, largCam, altCam, 0, 0, largImg, altImg, 0);
            al_draw_scaled_bitmap(imagem_dir, 0, 0, largCam, altCam, largImg, 0, largImg, altImg, 0);

            al_draw_textf(fonte, al_map_rgb(0, 0, 0), 20, ALT - 42, ALLEGRO_ALIGN_LEFT,
					"Range: %d Sensibilidade: %d  (setas para alterar)", range, sensibilidade);

            al_draw_textf(fonte, al_map_rgb(0,0,0), LARG/4 - 122 , 50, ALLEGRO_ALIGN_LEFT, "pontosPlayer1 %d", pontosPlayer1);

            al_draw_textf(fonte, al_map_rgb(0,0,0), LARG - (122 + LARG/4 ) , 50, ALLEGRO_ALIGN_LEFT, "pontosPlayer2 %d", pontosPlayer2);

            if(movimentoPlayer1 || movimentoPlayer2) {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT,
				    "Moveu!");
                parado = 0;
            } else {            

            	parado++;
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT,
    				"Parado %d",parado / 30);

            }

			al_flip_display();
			
		
			if((parado/30) >= 3){
				int ganhador = 0 ;
				
				while(ganhador != 1){

					camera_atualiza(cam);
					camera_atualiza(cam2);

					bool Player1_vencedor = busca_cor(cam, 1, matriz);
					bool Player2_vencedor = busca_cor(cam2, 1, matriz2);

					camera_copia(cam, cam->quadro, imagem_esq);
					//camera_copia(cam, matriz, imagem_dir);

					//para duas cameras substituir linha de cima por linha comentada
            		camera_copia(cam2, cam2->quadro, imagem_dir);

					al_draw_scaled_bitmap(imagem_esq, 0, 0, largCam, altCam, 0, 0, largImg, altImg, 0);
            		al_draw_scaled_bitmap(imagem_dir, 0, 0, largCam, altCam, largImg, 0, largImg, altImg, 0);

            		al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARG/4, ALT/4, ALLEGRO_ALIGN_LEFT,"ATIRE");

					al_flip_display();

					al_clear_to_color(al_map_rgb(0, 0, 0));

					if(Player1_vencedor && Player2_vencedor){
						camera_copia(cam, cam->quadro, imagem_esq);
						camera_copia(cam2, cam2->quadro, imagem_dir);

						al_draw_scaled_bitmap(imagem_esq, 0, 0, largCam, altCam, 0, 0, largImg, altImg, 0);
            			al_draw_scaled_bitmap(imagem_dir, 0, 0, largCam, altCam, largImg, 0, largImg, altImg, 0);

            			al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARG/2 - 50, ALT/2, ALLEGRO_ALIGN_LEFT, "DRAW");
						al_flip_display();

						al_rest(10.0);
						ganhador = 1;

					}
 	
					if(Player1_vencedor){
						camera_copia(cam, cam->quadro, imagem_esq);
						camera_copia(cam, cam->quadro, imagem_dir);
						al_draw_scaled_bitmap(imagem_esq, 0, 0, largCam, altCam, 0, 0, largImg, altImg, 0);
            			al_draw_scaled_bitmap(imagem_dir, 0, 0, largCam, altCam, largImg, 0, largImg, altImg, 0);

						al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARG/2 - 50, ALT/2, ALLEGRO_ALIGN_LEFT, "GANHADOR PLAYER 1");
						al_flip_display();

						al_rest(10.0);
						ganhador = 1;
						pontosPlayer1++;
					}		

					if(Player2_vencedor){
						camera_copia(cam2, cam2->quadro, imagem_esq);
						camera_copia(cam2, cam2->quadro, imagem_dir);
						al_draw_scaled_bitmap(imagem_esq, 0, 0, largCam, altCam, 0, 0, largImg, altImg, 0);
            			al_draw_scaled_bitmap(imagem_dir, 0, 0, largCam, altCam, largImg, 0, largImg, altImg, 0);
						al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARG/2 - 50, ALT/2, ALLEGRO_ALIGN_LEFT, "GANHADOR PLAYER 2");
						al_flip_display();

						al_rest(10.0);
						ganhador = 1;
						pontosPlayer2++;
					}

				}

				camera_atualiza(cam);
				camera_atualiza(cam2);
				al_flip_display();
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

		}
	}



    camera_libera_matriz(cam, matriz_anterior);
    camera_libera_matriz(cam, matriz);
    camera_finaliza(cam);

    //libera camera 2
    camera_libera_matriz(cam2, matriz_anterior2);
    camera_libera_matriz(cam2, matriz2);
    camera_finaliza(cam2);

    al_unregister_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_unregister_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_unregister_event_source(fila_de_eventos, al_get_mouse_event_source());
    al_unregister_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_destroy_event_queue(fila_de_eventos);

	al_destroy_font(fonte);

	al_stop_timer(timer);
	al_destroy_timer(timer);

    al_destroy_bitmap(imagem_esq);
    al_destroy_bitmap(imagem_dir);
    
	al_destroy_display(janela);

}



int main(void){

	jogo();


	return 0;
}
