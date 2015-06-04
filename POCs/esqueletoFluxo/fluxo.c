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

enum ESTADO{tela_titulo, tela_jogo, tela_fim, tela_como_jogar, tela_creditos, tela_escolha_rodadas}; 

int main (void) {

	//Inicialização de variaveis
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

    //Timagem de tela de titulo
    ALLEGRO_BITMAP *imagem_fundo_titulo = NULL;
    ALLEGRO_BITMAP *imagem_fundo_creditos = NULL;
	ALLEGRO_BITMAP *imagem_fundo_como_jogar = NULL;
	ALLEGRO_BITMAP *imagem_fundo_escolha_rodadas = NULL;

    ALLEGRO_FONT *fonteMedia = NULL;
    ALLEGRO_FONT *fonteMediaMenor = NULL;


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
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	//const int LARG = disp_data.width;
    const int LARG = 1280;
	//const int ALT = disp_data.height;
	const int ALT = 720;
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
	fonteMedia = al_load_font("fontes/thedeadsaloon-Regular.ttf", (5 * ALT) / 100, 0);
	if (!fonteMedia)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}

	fonteMediaMenor = al_load_font("fontes/thedeadsaloon-Regular.ttf", (4.5 * ALT) / 100, 0);
	if (!fonteMediaMenor)
	{
		fprintf(stderr, "Falha ao Carregar a Fonte\n");
		return EXIT_FAILURE;
	}

	imagem_fundo_titulo = al_load_bitmap("imagens/p_inicial.png");
	if (!imagem_fundo_titulo)
	{
		fprintf(stderr, "Falha ao carregar imagem 'p_inicial.png'\n");
		return EXIT_FAILURE;
	}

	imagem_fundo_creditos = al_load_bitmap("imagens/creditos.png");
	if (!imagem_fundo_creditos)
	{
		fprintf(stderr, "Falha ao carregar imagem 'creditos.png'\n");
		return EXIT_FAILURE;
	}

	imagem_fundo_como_jogar = al_load_bitmap("imagens/comojogar.png");
	if (!imagem_fundo_como_jogar)
	{
		fprintf(stderr, "Falha ao carregar imagem 'comojogar.png'\n");
		return EXIT_FAILURE;
	}

	imagem_fundo_escolha_rodadas = al_load_bitmap("imagens/selecao.png");
	if (!imagem_fundo_escolha_rodadas)
	{
		fprintf(stderr, "Falha ao carregar imagem 'selecao.png'\n");
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

		//EVENTO DE CLICK DO BOTAO DO MOUSE
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (evento.mouse.button & 1) {
				//CLICK NA TELA DE TITULO
				if (estado == tela_titulo) {
					// SE ELE CLICAR EM INICIAR JOGO
					if (mouseX >= LARG / 2 - (15 * LARG) / 100 && mouseX <= LARG / 2 + (15 * LARG) / 100 &&
					mouseY >= (59 * ALT) / 100 && mouseY <= (73 * ALT) / 100) {
						/*tempo = 0;
						tempoInicio = 0;
						permiteTiro = false;
						dueloIniciado = false;
						camera_atualiza(cam);
						camera_atualiza(cam02);
						estado = tela_jogo;*/
						estado = tela_escolha_rodadas;
					}
					// SE ELE CLICAR EM COMO JOGAR
					if (mouseX >= LARG / 2 - (12 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (74 * ALT) / 100 && mouseY <= (85 * ALT) / 100) {
						estado = tela_como_jogar;
					}

					// SE ELE CLICOU EM CREDITOS	
					if (mouseX >= LARG / 2 - (12 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (87 * ALT) / 100 && mouseY <= (98 * ALT) / 100) {
						estado = tela_creditos;
					}

				}else if (estado == tela_escolha_rodadas){
					//Se escolhe melhor de 1
					if (mouseX >= LARG / 2 - (43 * LARG) / 100 && mouseX <= LARG / 2 + (-20 * LARG) / 100 &&
						mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
						rounds = 1;
						estado = tela_jogo;
					}
					//Se escolhe melhor de 3
					if (mouseX >= LARG / 2 - (11 * LARG) / 100 && mouseX <= LARG / 2 + (11.3 * LARG) / 100 &&
						mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
						rounds = 3;
						estado = tela_jogo;
					}

					//Se escolhe melhor de 5
					if (mouseX >= LARG / 2 - (- 21 * LARG) / 100 && mouseX <= LARG / 2 + (43 * LARG) / 100 &&
						mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
						rounds = 5;	
						estado = tela_jogo;
					}

				}else if (estado == tela_como_jogar) {
					//BOTAO DE VOLTAR
					if (mouseX >= LARG / 2 - (14 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
						mouseY >= (80 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
						estado = tela_titulo;
					}
				}else if (estado == tela_creditos) {
					// SE ELE CLICOU NO BOTAO VOLTAR
					if (mouseX >= LARG / 2 - (14 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (80 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
						estado = tela_titulo;
					}
				}
			}
		}

		if (renderizar && al_is_event_queue_empty(fila_de_eventos)) {
			renderizar = false;

			if (estado == tela_titulo) {

				al_draw_bitmap(imagem_fundo_titulo, 0, 0, 0);

				/* BOTAO DE INICIAR JOGO */
				if (mouseX >= LARG / 2 - (15 * LARG) / 100 && mouseX <= LARG / 2 + (15 * LARG) / 100 &&
					mouseY >= (59 * ALT) / 100 && mouseY <= (73 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (15 * LARG) / 100, (59 * ALT) / 100, LARG / 2 + (15 * LARG) / 100,
					 (73 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}
				

				// BOTAO DE COMO JOGAR
				if (mouseX >= LARG / 2 - (12 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (74 * ALT) / 100 && mouseY <= (85 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (12 * LARG) / 100, (74 * ALT) / 100, LARG / 2 + (12 * LARG) / 100,
					 (85 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}

				//BOTAO DE TELA DE CREDITOS
				if (mouseX >= LARG / 2 - (12 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (87 * ALT) / 100 && mouseY <= (98 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (12 * LARG) / 100, (87 * ALT) / 100, LARG / 2 + (12 * LARG) / 100, (98 * ALT) / 100, 
						al_map_rgb(255, 255, 255), 5);
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

			} else if (estado == tela_escolha_rodadas) {
				al_draw_bitmap(imagem_fundo_escolha_rodadas, 0, 0, 0);

				//Se escolhe melhor de 1
				if (mouseX >= LARG / 2 - (43 * LARG) / 100 && mouseX <= LARG / 2 + (-20 * LARG) / 100 &&
					mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (43 * LARG) / 100, (30 * ALT) / 100, LARG / 2 + (-20 * LARG) / 100,
						(70 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}
				//Se escolhe melhor de 3
				if (mouseX >= LARG / 2 - (11 * LARG) / 100 && mouseX <= LARG / 2 + (11.3 * LARG) / 100 &&
					mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (11 * LARG) / 100, (30 * ALT) / 100, LARG / 2 + (11.3 * LARG) / 100,
						(70 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}

				//Se escolhe melhor de 5
				if (mouseX >= LARG / 2 - (- 21 * LARG) / 100 && mouseX <= LARG / 2 + (43 * LARG) / 100 &&
					mouseY >= (30 * ALT) / 100 && mouseY <= (70 * ALT) / 100) {
				
					al_draw_rectangle(LARG / 2 - (- 21 * LARG) / 100, (30 * ALT) / 100, LARG / 2 + (43 * LARG) / 100,
						(70 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}
			} else if (estado == tela_como_jogar) {
				al_draw_bitmap(imagem_fundo_como_jogar, 0, 0, 0);

				al_draw_textf(fonte22, al_map_rgb(0, 0, 0), LARG / 2 - 250 , ALT / 2 - 150, ALLEGRO_ALIGN_LEFT,
					    "•");
				al_draw_textf(fonteMediaMenor, al_map_rgb(0, 0, 0), LARG / 2 - 200 , ALT / 2 - 150, ALLEGRO_ALIGN_LEFT,
					    "Escolha quantidade de rodadas");
				al_draw_textf(fonteMediaMenor, al_map_rgb(0, 0, 0), LARG / 2 - 200, ALT / 2 - 100, ALLEGRO_ALIGN_LEFT,
					    "Fique sem se mexer diante a camera");
				al_draw_textf(fonteMediaMenor, al_map_rgb(0, 0, 0), LARG / 2 - 200, ALT / 2 - 50, ALLEGRO_ALIGN_LEFT,
					    "Aguarde o sinal ");
				al_draw_textf(fonteMediaMenor, al_map_rgb(0, 0, 0), LARG / 2 - 200, ALT / 2 , ALLEGRO_ALIGN_LEFT,
					    "Atire no oponente");

				//BOTAO DE VOLTAR
				if (mouseX >= LARG / 2 - (14 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (80 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (14 * LARG) / 100, (80 * ALT) / 100, LARG / 2 + (12 * LARG) / 100,
					 (90 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
				}

			} else if (estado == tela_creditos) {
				// Desenha tela de creditos
				al_draw_bitmap(imagem_fundo_como_jogar, 0, 0, 0);

				al_draw_textf(fonteMedia, al_map_rgb(0, 0, 0), LARG / 2 , ALT / 2 - 200, ALLEGRO_ALIGN_CENTRE,
					    "Jogo desenvolvido por ");
				al_draw_textf(fonteMedia, al_map_rgb(0, 0, 0), LARG / 2 , ALT / 2 - 100, ALLEGRO_ALIGN_CENTRE,
					    "Mario de Castro");
				al_draw_textf(fonteMedia, al_map_rgb(0, 0, 0), LARG / 2 , ALT / 2 - 50, ALLEGRO_ALIGN_CENTRE,
					    "William Alvelos");
				al_draw_textf(fonteMedia, al_map_rgb(0, 0, 0), LARG / 2 , ALT / 2 , ALLEGRO_ALIGN_CENTRE,
					    "Humberto Vieira");
				al_draw_textf(fonteMedia, al_map_rgb(0, 0, 0), LARG / 2 , ALT / 2 + 100 , ALLEGRO_ALIGN_CENTRE,
					    "Designer Fernanda Ruivo");

				//BOTAO DE VOLTAR
				if (mouseX >= LARG / 2 - (14 * LARG) / 100 && mouseX <= LARG / 2 + (12 * LARG) / 100 &&
					mouseY >= (80 * ALT) / 100 && mouseY <= (90 * ALT) / 100) {
					al_draw_rectangle(LARG / 2 - (14 * LARG) / 100, (80 * ALT) / 100, LARG / 2 + (12 * LARG) / 100,
					 (90 * ALT) / 100, al_map_rgb(255, 255, 255), 5);
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
	al_destroy_font(fonteMedia);
	al_destroy_font(fonteMediaMenor);

	al_destroy_bitmap(imagem_esq);
    al_destroy_bitmap(imagem_dir);

    al_destroy_bitmap(imagem_fundo_titulo);
    al_destroy_bitmap(imagem_fundo_creditos);
    al_destroy_bitmap(imagem_fundo_como_jogar);

	al_stop_timer(timer);
	al_destroy_timer(timer);
    
	al_destroy_display(janela);

    return 0;
}
