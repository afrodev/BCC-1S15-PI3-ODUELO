#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "camera.h"

void copia_matriz(camera *cam, unsigned char ***matriz_01, unsigned char ***matriz_02) {
	for (int y = 0; y < cam->altura; y++) {
        for (int x = 0; x < cam->largura; x++) {
            matriz_02[y][x][0] = matriz_01[y][x][0];
            matriz_02[y][x][1] = matriz_01[y][x][1];
            matriz_02[y][x][2] = matriz_01[y][x][2];
        }
    };
}

int main (void) {
	bool finalizado = false;
	bool renderizar = true;
    bool movimento = false;
    int diferenca = 0;
    int sensibilidade = 4;
	int range = 70;
	const int FPS = 60;
        
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
	ALLEGRO_BITMAP *imagem_esq = NULL;
    ALLEGRO_BITMAP *imagem_meio = NULL;
    ALLEGRO_BITMAP *imagem_dir = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fonte22 = NULL;

	camera *cam = camera_inicializa(0);
	if (!cam) {
		fprintf(stderr, "Falha ao Inicializar a Camera\n");
		return EXIT_FAILURE;
	}

	const int LARG = cam->largura * 3;
	const int ALT = cam->altura;

	unsigned char ***matriz = camera_aloca_matriz(cam);
    unsigned char ***matriz_original = camera_aloca_matriz(cam);

	if (!al_init()) {
		fprintf(stderr, "Falha ao Carregar Allegro5\n");
		return EXIT_FAILURE;
	}

	al_get_display_mode(0, &disp_data);
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

	fonte22 = al_load_font("georgiab.ttf", 22, 0);
	if (!fonte22)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}

	imagem_esq = al_create_bitmap(cam->largura, cam->altura);
    imagem_meio = al_create_bitmap(cam->largura, cam->altura);
    imagem_dir = al_create_bitmap(cam->largura, cam->altura);

	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

	al_start_timer(timer);
    camera_atualiza(cam);
    copia_matriz(cam, cam->quadro, matriz_original);
    camera_copia(cam, matriz_original, imagem_dir);

	while(!finalizado) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_de_eventos, &evento);

		if (evento.type == ALLEGRO_EVENT_TIMER && !renderizar) {
			renderizar = true;
			camera_atualiza(cam);

            diferenca = 0;
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
                movimento = true;
            } else {
                movimento = false;
            }
		} else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			finalizado = true;
		} else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					finalizado = true;
					break;
				case ALLEGRO_KEY_SPACE:
					copia_matriz(cam, cam->quadro, matriz_original);
					camera_copia(cam, matriz_original, imagem_dir);
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
                    sensibilidade += 2;
                    break;
                case ALLEGRO_KEY_LEFT:
                    sensibilidade -= 2;
                    if (sensibilidade < 2)
                        sensibilidade = 2;
                    break;
			}
		}
		if (renderizar && al_is_event_queue_empty(fila_de_eventos)) {
			renderizar = false;

			camera_copia(cam, cam->quadro, imagem_esq);
            camera_copia(cam, matriz, imagem_meio);

            al_draw_bitmap(imagem_esq, 0, 0, 0);
            al_draw_bitmap(imagem_meio, LARG / 3, 0, 0);
            al_draw_bitmap(imagem_dir, (2 * LARG) / 3, 0, 0);

            al_draw_textf(fonte22, al_map_rgb(0, 0, 0), 20, ALT - 42, ALLEGRO_ALIGN_LEFT,
					"Range: %d Sensibilidade: %d (setas para alterar)", range, sensibilidade);
            al_draw_textf(fonte22, al_map_rgb(0, 0, 0), LARG - 20, ALT - 42, ALLEGRO_ALIGN_RIGHT,
					"Espaco: resetar imagem");

            if (movimento) {
                al_draw_textf(fonte22, al_map_rgb(0, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT,
				    "Moveu!");
            } else {
                al_draw_textf(fonte22, al_map_rgb(0, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT,
				    "Parado");
            }

            al_draw_textf(fonte22, al_map_rgb(0, 0, 0), 20, 50, ALLEGRO_ALIGN_LEFT,
                    "%d %d", diferenca, (cam->altura * cam->largura) / sensibilidade);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

    camera_libera_matriz(cam, matriz_original);
    camera_libera_matriz(cam, matriz);
    camera_finaliza(cam);

    al_unregister_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_unregister_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_unregister_event_source(fila_de_eventos, al_get_mouse_event_source());
    al_unregister_event_source(fila_de_eventos, al_get_keyboard_event_source());
	al_destroy_event_queue(fila_de_eventos);

	al_destroy_font(fonte22);

	al_stop_timer(timer);
	al_destroy_timer(timer);

    al_destroy_bitmap(imagem_esq);
    al_destroy_bitmap(imagem_meio);
    al_destroy_bitmap(imagem_dir);
    
	al_destroy_display(janela);

    return 0;
}
