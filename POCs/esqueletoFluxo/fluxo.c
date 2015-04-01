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
 * Esqueleto da fluxo de telas do Jogo
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "camera.h"
#include "duelolib.h"

enum ESTADO{tela_titulo, tela_jogo}; 

int main (void) {
	int estado = tela_titulo;
	bool finalizado = false;
	bool renderizar = true;
	const int FPS = 60;

	float mouseX;
	float mouseY;
        
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fonteTitulo = NULL;
	ALLEGRO_FONT *fonteGrande = NULL;

	camera *cam = camera_inicializa(0);
	if (!cam) {
		fprintf(stderr, "Falha ao Inicializar a Camera\n");
		return EXIT_FAILURE;
	}

	if (!al_init()) {
		fprintf(stderr, "Falha ao Carregar Allegro5\n");
		return EXIT_FAILURE;
	}

	al_get_display_mode(0, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	const int LARG = disp_data.width;
    //const int LARG = 1280;
	const int ALT = disp_data.height;
	//const int ALT = 720;
	float razao = (float) LARG / ALT;
	janela = al_create_display(LARG, ALT);
	if (!janela) {
		fprintf(stderr, "Falha ao Iniciar Janela\n");
		return EXIT_FAILURE;
	}
	al_set_window_position(janela, (disp_data.width - LARG) / 2, (disp_data.height - ALT) / 2);
	al_set_window_title(janela, "POC");

	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	fila_de_eventos = al_create_event_queue();
	if (!fila_de_eventos) {
		fprintf(stderr, "Falha ao Criar Fila de Eventos\n");
		return EXIT_FAILURE;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Falha ao Criar Timer\n");
		return EXIT_FAILURE;
	}

	fonteTitulo = al_load_font("georgiab.ttf", (13 * ALT) / 100, 0);
	if (!fonteTitulo)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}
	fonteGrande = al_load_font("georgiab.ttf", (6 * ALT) / 100, 0);
	if (!fonteGrande)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}

	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

	al_start_timer(timer);

	while(!finalizado) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_de_eventos, &evento);

		if (evento.type == ALLEGRO_EVENT_TIMER && !renderizar) {
			renderizar = true;
		}

		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			finalizado = true;
		}

		else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					// if (estado == tela_titulo)
						finalizado = true;
					break;
				case ALLEGRO_KEY_SPACE:
					break;
				case ALLEGRO_KEY_UP:
					break;
				case ALLEGRO_KEY_DOWN:
					break;
                case ALLEGRO_KEY_RIGHT:
                    break;
                case ALLEGRO_KEY_LEFT:
                    break;
			}
		}

		else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
			mouseX = evento.mouse.x;
			mouseY = evento.mouse.y;
		}

		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (evento.mouse.button & 1) {
				if (estado == tela_titulo) {
					if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
						mouseY >= (37 * ALT) / 100 && mouseY <= (50 * ALT) / 100) {
						estado = tela_jogo;
					}
					if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
						mouseY >= (77 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
						finalizado = true;
					}
				}
			}
		}

		if (renderizar && al_is_event_queue_empty(fila_de_eventos)) {
			renderizar = false;

			if (estado == tela_titulo) {
				al_draw_textf(fonteTitulo, al_map_rgb(240, 240, 240), LARG / 2, (10 * ALT) / 100, ALLEGRO_ALIGN_CENTRE, "O DUELO");

				al_draw_filled_rectangle(LARG / 2 - (13 * LARG) / 100, (37 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (50 * ALT) / 100, al_map_rgb(20, 20, 20));
				al_draw_textf(fonteGrande, al_map_rgb(240, 240, 240), LARG / 2, (40 * ALT) / 100, ALLEGRO_ALIGN_CENTRE, "Iniciar Jogo");
				if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
					mouseY >= (37 * ALT) / 100 && mouseY <= (50 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (13 * LARG) / 100, (37 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (50 * ALT) / 100, al_map_rgb(60, 60, 60), 5);
				}
				
				al_draw_filled_rectangle(LARG / 2 - (13 * LARG) / 100, (57 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (70 * ALT) / 100, al_map_rgb(20, 20, 20));
				al_draw_textf(fonteGrande, al_map_rgb(240, 240, 240), LARG / 2, (60 * ALT) / 100, ALLEGRO_ALIGN_CENTRE, "Como Jogar");
				if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
					mouseY >= (57 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (13 * LARG) / 100, (57 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (70 * ALT) / 100, al_map_rgb(60, 60, 60), 5);
				}

				al_draw_filled_rectangle(LARG / 2 - (13 * LARG) / 100, (77 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (90 * ALT) / 100, al_map_rgb(20, 20, 20));
				al_draw_textf(fonteGrande, al_map_rgb(240, 240, 240), LARG / 2, (80 * ALT) / 100, ALLEGRO_ALIGN_CENTRE, "Sair");
				if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
					mouseY >= (77 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (13 * LARG) / 100, (77 * ALT) / 100, LARG / 2 + (13 * LARG) / 100, (90 * ALT) / 100, al_map_rgb(60, 60, 60), 5);
				}
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(39, 40, 34));
		}
	}

    camera_finaliza(cam);

    al_unregister_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_unregister_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_unregister_event_source(fila_de_eventos, al_get_mouse_event_source());
    al_unregister_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_destroy_event_queue(fila_de_eventos);

	al_destroy_font(fonteTitulo);
	al_destroy_font(fonteGrande);

	al_stop_timer(timer);
	al_destroy_timer(timer);
    
	al_destroy_display(janela);

    return 0;
}
