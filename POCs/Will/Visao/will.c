#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "camera.h"


void inicializa_visao(){


	camera * cam = camera_inicializa(0);


	int largura = cam->largura;
	int altura = cam->altura;


	al_init();

	ALLEGRO_DISPLAY *display = al_create_display(largura, altura);
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

	al_register_event_source(queue, al_get_display_event_source(display));

 	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
	
	if(!timer)
    	printf("nao foi possivel criar relogio");

  	al_register_event_source(queue, al_get_timer_event_source(timer));


  	unsigned char ***matriz = camera_aloca_matriz(cam);

  	ALLEGRO_COLOR cor = al_map_rgb_f(0, 0, 1);

  	//ALLEGRO_BITMAP *tela = al_get_backbuffer(display);

  	//ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap(tela, 0, 0, largura, altura);

  	camera_copia(cam, display);
}

int main(void){


	inicializa_visao();




	return 0;
}