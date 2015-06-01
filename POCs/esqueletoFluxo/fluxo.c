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
#include <time.h> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "camera.h"
#include "duelolib.h"

typedef struct{
	float h;
	float s;
	float v;
}hsv;

enum ESTADO{tela_titulo, tela_jogo, tela_fim}; 

int main (void) {
	int estado = tela_titulo;
	bool finalizado = false;
	bool renderizar = true;
	const int FPS = 60;
	int tempo = 0;
	int tempoInicio = 0;
	int rounds;

	bool corPlayer01 = false;
	bool corPlayer02 = false;

	bool movimento01 = false;
	bool movimento02 = false;

	bool permiteTiro = false;
	bool dueloIniciado = false;

    int sensibilidade = 150;
    int sensibilidadeCor = 75;
	int range = 40;

	float mouseX;
	float mouseY;
        
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fonteTitulo = NULL;
	ALLEGRO_FONT *fonteGrande = NULL;
	ALLEGRO_FONT *fonte22 = NULL;
	ALLEGRO_BITMAP *imagem_esq = NULL;
    ALLEGRO_BITMAP *imagem_dir = NULL;	

	camera *cam = camera_inicializa(0);
	if (!cam) {
		fprintf(stderr, "Falha ao Inicializar a Camera\n");
		return EXIT_FAILURE;
	}

	camera *cam02 = camera_inicializa(1);
	if (!cam02) {
		fprintf(stderr, "Falha ao Inicializar a Camera 02\n");
		return EXIT_FAILURE;
	}

	unsigned char ***matriz = camera_aloca_matriz(cam);
    unsigned char ***matriz_anterior = camera_aloca_matriz(cam);
    unsigned char ***matriz02 = camera_aloca_matriz(cam02);
    unsigned char ***matriz_anterior02 = camera_aloca_matriz(cam02);

	if (!al_init()) {
		fprintf(stderr, "Falha ao Carregar Allegro5\n");
		return EXIT_FAILURE;
	}

	int lixo = al_get_num_display_modes() - 1;
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

	float largImg = disp_data.width / 2;
	float altImg = (float) cam->altura * largImg / cam->largura;

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
	fonte22 = al_load_font("georgiab.ttf", (2 * ALT) / 100, 0);
	if (!fonte22)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}

	imagem_esq = al_create_bitmap(cam->largura, cam->altura);
    imagem_dir = al_create_bitmap(cam02->largura, cam02->altura);

	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

	al_start_timer(timer);
	srand(time(NULL));

	camera_atualiza(cam);
	copia_matriz(cam, cam->quadro, matriz_anterior);
	camera_atualiza(cam02);
	copia_matriz(cam02, cam02->quadro, matriz_anterior02);

	while(!finalizado) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_de_eventos, &evento);

		if (evento.type == ALLEGRO_EVENT_TIMER && !renderizar) {
			renderizar = true;

			camera_atualiza(cam);
			camera_atualiza(cam02);

			if (estado == tela_jogo) {
				corPlayer01 = false;
				corPlayer02 = false;
				movimento01 = compara_matriz(cam, matriz_anterior, matriz, range, sensibilidade, sensibilidadeCor, &corPlayer01);
				movimento02 = compara_matriz(cam02, matriz_anterior02, matriz02, range, sensibilidade, sensibilidadeCor, &corPlayer02);

				if (!dueloIniciado && !movimento01 && !movimento02) {
					tempoInicio++;
				} else {
					tempoInicio = 0;
				}

				if (tempoInicio > 10) {
					dueloIniciado = true;
				}

				if (dueloIniciado) {
					if (!permiteTiro) {
						if (movimento01 || movimento02) {
							estado = tela_fim;
						}
					}

					tempo++;	
	            	if ((tempo > 20 && (rand() % 25 == 0)) || tempo > 60) {
	            		permiteTiro = true;
	            	}

	            	if (permiteTiro && (corPlayer01 || corPlayer02)) {
	            		estado = tela_fim;
	            	}
				}

            	copia_matriz(cam, cam->quadro, matriz_anterior);
            	copia_matriz(cam02, cam02->quadro, matriz_anterior02);
			}
		}

		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			finalizado = true;
		}

		else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					if (estado == tela_titulo)
						finalizado = true;
					else
						estado = tela_titulo;
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

		else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
			mouseX = evento.mouse.x;
			mouseY = evento.mouse.y;
		}

		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (evento.mouse.button & 1) {
				if (estado == tela_titulo) {
					if (mouseX >= LARG / 2 - (13 * LARG) / 100 && mouseX <= LARG / 2 + (13 * LARG) / 100 &&
						mouseY >= (37 * ALT) / 100 && mouseY <= (50 * ALT) / 100) {
						tempo = 0;
						tempoInicio = 0;
						permiteTiro = false;
						dueloIniciado = false;
						camera_atualiza(cam);
						camera_atualiza(cam02);
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
			} else if (estado == tela_jogo) {
				camera_copia(cam, cam->quadro, imagem_esq);
	            camera_copia(cam02, cam02->quadro, imagem_dir);	

	            al_draw_scaled_bitmap(imagem_esq, 0, 0, cam->largura, cam->altura, 0, (ALT / 2) - (altImg / 2), largImg, altImg, 0);
	            al_draw_scaled_bitmap(imagem_dir, 0, 0, cam02->largura, cam02->altura, largImg, (ALT / 2) - (altImg / 2), largImg, altImg, 0);

	            al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, ALT - 42, ALLEGRO_ALIGN_LEFT,
					"Range: %d Sensibilidade: %d  (setas para alterar) tempoInicio: %d tempo: %d", range, sensibilidade, tempoInicio, tempo);

	            if (permiteTiro) {
	            	al_draw_textf(fonteGrande, al_map_rgb(255, 0, 0), LARG / 2, ALT / 2, ALLEGRO_ALIGN_CENTRE,
					    "ATIRE!");
	            } else if (dueloIniciado) {
	            	al_draw_textf(fonteGrande, al_map_rgb(255, 0, 0), LARG / 2, ALT / 2, ALLEGRO_ALIGN_CENTRE,
					    "PREPARAR!");
	            } else {
	            	al_draw_textf(fonteGrande, al_map_rgb(255, 0, 0), LARG / 2, ALT / 2, ALLEGRO_ALIGN_CENTRE,
					    "NÂO SE MOVA!");
	            }

	            if (movimento01) {
	                al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
					    "Moveu!");
	            } else {            
	                al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
	    				"Parado");
	            }
	            if (movimento02) {
	                al_draw_textf(fonte22, al_map_rgb(255, 255, 255), LARG - 20, 20, ALLEGRO_ALIGN_RIGHT,
					    "Moveu!");
	            } else {            
	                al_draw_textf(fonte22, al_map_rgb(255, 255, 255), LARG - 20, 20, ALLEGRO_ALIGN_RIGHT,
	    				"Parado");
	            }
			} else if (estado == tela_fim) {
				if (corPlayer01) {
					al_draw_scaled_bitmap(imagem_esq, 0, 0, cam->largura, cam->altura, (LARG / 2) - (largImg / 2), (ALT / 2) - (altImg / 2), largImg, altImg, 0);
					al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
	    				"Jogador 1 Ganhou!");
				} else if (corPlayer02) {
					al_draw_scaled_bitmap(imagem_dir, 0, 0, cam->largura, cam->altura, (LARG / 2) - (largImg / 2), (ALT / 2) - (altImg / 2), largImg, altImg, 0);
					al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
	    				"Jogador 2 Ganhou!");
				} else if (!permiteTiro && movimento01) {
					al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
	    				"Jogador 1 se moveu antes da hora!");
				} else if (!permiteTiro && movimento02) {
					al_draw_textf(fonte22, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT,
	    				"Jogador 2 se moveu antes da hora!");
				}
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(39, 40, 34));
		}
	}

    camera_libera_matriz(cam02, matriz_anterior02);
    camera_libera_matriz(cam02, matriz_anterior02);
    camera_libera_matriz(cam, matriz);
    camera_libera_matriz(cam, matriz);
    camera_finaliza(cam02);
    camera_finaliza(cam);

    al_unregister_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_unregister_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_unregister_event_source(fila_de_eventos, al_get_mouse_event_source());
    al_unregister_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_destroy_event_queue(fila_de_eventos);

	al_destroy_font(fonteTitulo);
	al_destroy_font(fonteGrande);
	al_destroy_font(fonte22);

	al_destroy_bitmap(imagem_esq);
    al_destroy_bitmap(imagem_dir);

	al_stop_timer(timer);
	al_destroy_timer(timer);
    
	al_destroy_display(janela);

    return 0;
}
