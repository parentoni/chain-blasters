#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "config.h"
#include "sprites.h"
#include "enemy.h"
#include "hero.h"
#include "attack.h"
#include "bar.h"

#define NUM_ENEMIES 15
#define TIRO_INATIVO 0
#define TIRO_ATIVO 1
#define RAIO_TIRO 100
#define TEMPO_TIRO 2.0
#define BORDA_TIRO 4
#define SCORE_PENALTY 0.3

ALLEGRO_COLOR BKG_COLOR;
ALLEGRO_FONT *FONT_32;

void drawScenario() {
	al_clear_to_color(al_map_rgb(10, 10, 10));
}

 
int main(int argc, char **argv){

	srand(time(NULL));

	int i;
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_init_font_addon();
	al_init_image_addon();
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}


	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//----------------------jogo-------------------

	//inicializa globais
	FONT_32 = al_load_font("./static/minecraft.ttf", 32, 1);   
	char* sprites[] = {"./static/hero.png", "./static/enemy.png", "./static/experience_bar.png"};
	intialize_sprites(sprites, 3);	

	hero h; game_state gs;

	// incializa jogo 
	initialize_hero(&h);
	initialize_game_state(&gs);
	initialize_enemies();
	initialize_waves();
	initialize_bullets();

	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			gs.tick_count++;

			if (gs.tick_count % FPS == 0) update_points_game_state(&gs, POINTS_PER_SECOND); // se passou 1 sec a gnt adiciona os pontos, adicionar continuamente (aka once per frame) seria estranho
			drawScenario();

			update_state_hero(&h);
			update_state_attack(&gs);
			update_state_wave(&gs, &h);
			playing = !update_state_enemies(&h, &gs);
			playing = !(!playing || update_state_bullets(&h));


			render_enemies(1);
			render_attacks();
			render_hero(&h, 0);
			render_bullets();
			render_bar(&gs, 2, FONT_32);	



			//pausa o jogo por 3 segundos se o jogador morrer
			if(!playing)
				al_rest(1);

			gs.tick_count %= 2000000;

		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			handle_key_down_hero(&h, &gs, ev.keyboard.keycode);
		}			

		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			handle_key_up_hero(&h, ev.keyboard.keycode);

		}

	} //fim do while
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	

	char my_text[100];
	al_clear_to_color(al_map_rgb(0,0,0));
	sprintf(my_text, "Pontuação: %d", (int)gs.points);
	al_draw_text(FONT_32, al_map_rgb(220, 30, 0), SCREEN_W/3, SCREEN_H/2 + 50, 0, my_text);


	al_flip_display();
	al_rest(3);	
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}