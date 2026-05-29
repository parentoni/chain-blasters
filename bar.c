#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <string.h>
#include <math.h>

#include "bar.h"
#include "sprites.h"

//#define BAR_R_PADDING 639
//#define BAR_PIXELS_PER_LEVEL 210
//#define BAR_SEPARATOR_WIDTH 4
//#define BAR_HEIGHT 40
//#define BAR_U_PADDING 34 
//#define BAR_INTERNAL_H_PADDING 2

int len_number(int n) {
	return floor(log10((float)n) + 1);
}

void render_bar(game_state* gs, int bar_sprite_index, ALLEGRO_FONT* f) {
	al_draw_bitmap(get_bitmap_by_index(bar_sprite_index), 0, 0, 0);
	int complete_levels = (int) gs->attack_points/ATTACK_POINTS_COST; 


	for (int i=0;i<complete_levels;i++) {
		int startx = BAR_R_PADDING + i * BAR_SEPARATOR_WIDTH + i * BAR_PIXELS_PER_LEVEL;
		al_draw_filled_rectangle(startx,
			BAR_U_PADDING + BAR_INTERNAL_H_PADDING,
			startx + BAR_PIXELS_PER_LEVEL,
			BAR_U_PADDING + BAR_HEIGHT - BAR_INTERNAL_H_PADDING,
			al_map_rgb(225,225,60));
	}

	int finishx = BAR_R_PADDING + complete_levels * BAR_SEPARATOR_WIDTH + complete_levels * BAR_PIXELS_PER_LEVEL;
	al_draw_filled_rectangle(finishx, 
			BAR_U_PADDING + BAR_INTERNAL_H_PADDING,
			finishx + BAR_PIXELS_PER_LEVEL * ((gs->attack_points % ATTACK_POINTS_COST)/(float)ATTACK_POINTS_COST),
			BAR_U_PADDING + BAR_HEIGHT - BAR_INTERNAL_H_PADDING,
			al_map_rgb(45,105,245)
	);

	int points_len = 9, t = gs->points;
	char points[20] = "Pontos: ";
	char points_n_txt[points_len + 1];  
	points_n_txt[points_len] = '\0';

	for (int i=0;i<points_len;i++) {
		points_n_txt[points_len - i - 1] = '0' + t % 10;
		t /= 10;
	}

	int wave_len = 3;
	t = gs->level;
	
	char wave[20] = "Waves: ";
	char wave_n_txt[wave_len + 1];  
	wave_n_txt[wave_len] = '\0';

	for (int i=0;i<wave_len;i++) {
		wave_n_txt[wave_len - i - 1] = '0' + t % 10;
		t /= 10;
	}

 	strcat(points, points_n_txt);
 	strcat(wave, wave_n_txt);

	al_draw_text(f, al_map_rgb(240, 240, 240), 50, BAR_U_PADDING, 0, points);
	al_draw_text(f, al_map_rgb(240, 240, 240), 50, BAR_U_PADDING + 2*32, 0, wave);
}
