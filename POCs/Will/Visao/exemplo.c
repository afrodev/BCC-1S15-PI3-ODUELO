#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "camera.h"



#define FPS 60

ALLEGRO_BITMAP *sniper = NULL;



void erro(char *mensagem) {
  fprintf(stderr, "%s\n", mensagem);

  exit(EXIT_FAILURE);
}


unsigned char *** compara_imagem(camera * cam, unsigned char ***matriz0, unsigned char ***matriz1){
  for(int i = 0; i < cam->altura; i++){
    for(int j = 0; j < cam->largura; j++){
      //int diferenca[2] = 0;
      int total_diferenca = 0;

      //diferenca[0] = matriz0[i][j][0] - matriz1[i][j][0];
      //diferenca[1] = matriz0[i][j][1] - matriz1[i][j][1];
      //diferenca[2] = matriz0[i][j][2] - matriz1[i][j][2];

      total_diferenca = (matriz0[i][j][0] - matriz1[i][j][0]) + (matriz0[i][j][1] - matriz1[i][j][1]) + (matriz0[i][j][2] - matriz1[i][j][2]); 
      
      if(total_diferenca > -30 && total_diferenca < 30){
        matriz1[i][j][0] = 0;
        matriz1[i][j][1] = 0;
        matriz1[i][j][2] = 0;

        //return true;
      }  

      else{
        matriz1[i][j][0] = 255;
        matriz1[i][j][1] = 255;
        matriz1[i][j][2] = 255;
        //return false;
      }

    }

  }

  return matriz1;
}


int main() {
  camera *cam = camera_inicializa(0);
  if(!cam)
    erro("nao foi possivel inicializar camera");

  int largura = cam->largura;
  int altura = cam->altura;

  if(!al_init())
    erro("nao foi possivel inicializar allegro");

  if(!al_init_primitives_addon())
    erro("nao foi possivel inicializar adicional de primitivas");

  if(!al_init_image_addon())
    erro("nao foi possivel inicializar a adicional de imagem ");

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  if(!queue)
    erro("nao foi possivel criar fila de eventos");

  ALLEGRO_DISPLAY *display = al_create_display(2 * largura, altura * 2);
  if(!display)
    erro("nao foi possivel criar janela");

  al_register_event_source(queue, al_get_display_event_source(display));

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  if(!timer)
    erro("nao foi possivel criar relogio");

  al_register_event_source(queue, al_get_timer_event_source(timer));

  /**********/

  unsigned char ***matriz = camera_aloca_matriz(cam);
  unsigned char ***anterior;

  ALLEGRO_COLOR cor = al_map_rgb_f(0, 0, 1);

  ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);

  ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap(buffer, 0, 0, largura, altura);

  ALLEGRO_BITMAP *direita = al_create_sub_bitmap(buffer, largura, 0, largura, altura);

  ALLEGRO_BITMAP * baixo = al_create_sub_bitmap(buffer, largura/2, altura, largura, altura);

  sniper = al_load_bitmap("semi.png");

  /**********/

  int continuar = 1;
  int atualizar = 0;

  al_start_timer(timer);

  while(continuar) {
    ALLEGRO_EVENT event;

    al_wait_for_event(queue, &event);

    switch(event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      continuar = 0;
      break;
    case ALLEGRO_EVENT_TIMER:
      atualizar = 1;
      break;
    }

    if(atualizar && al_is_event_queue_empty(queue)) {
      camera_atualiza(cam);

      /**********/

      int cy = 0;
      int cx = 0;
      int cn = 0;

      for(int y = 0; y < altura; y++)
        for(int x = 0; x < largura; x++) {
          float r = (cam->quadro[y][x][0]) / 255;
          float g = (cam->quadro[y][x][1]) / 255;
          float b = (cam->quadro[y][x][2]) / 255;

          /*if(compara_imagem(cam, matriz, anterior)) {
            cy += y;
            cx += x;
            cn += 1;

            matriz[y][x][0] = 255;
            matriz[y][x][1] = 255;
            matriz[y][x][2] = 255;
          }
          else {
            matriz[y][x][0] = 0;
            matriz[y][x][1] = 0;
            matriz[y][x][2] = 0;
          }
        }
        */

      anterior = compara_imagem(cam, matriz, anterior);
        //int x1 = cx / cn;
        //int y1 = cy / cn;
        
      /**********/
      }
      camera_copia(cam, cam->quadro, esquerda);
      camera_copia(cam, matriz, direita);
      camera_copia(cam, anterior, baixo);   
      if(cn > 0){
        //al_draw_circle(cx / cn, cy / cn, 120, cor, 1);
        //al_draw_bitmap(sniper, cx / cn, cy / cn, 120);
        // SNIPER al_draw_bitmap(sniper, cx / cn - 400, cy / cn - 300, 120);
        

      camera_copia(cam, cam->quadro, direita);

      /**********/

      al_flip_display();

      atualizar = 0;
    }
  }
}
  al_stop_timer(timer);

  /**********/

  al_destroy_bitmap(direita);

  al_destroy_bitmap(esquerda);

  camera_libera_matriz(cam, matriz);

  /**********/

  al_unregister_event_source(queue, al_get_timer_event_source(timer));
  al_destroy_timer(timer);

  al_unregister_event_source(queue, al_get_display_event_source(display));
  al_destroy_display(display);

  al_destroy_event_queue(queue);
  al_shutdown_primitives_addon();
  al_uninstall_system();

  camera_finaliza(cam);

  return EXIT_SUCCESS;


}