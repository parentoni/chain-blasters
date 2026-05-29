#ifndef HERO
#define HERO

#include "config.h"

typedef struct {
	vec2 pos;
	vec2 velocity; //
} hero;

void initialize_hero(hero* h);

void update_state_hero(hero* h);
void render_hero(hero* h, int hero_bitmap_index);

void handle_key_down_hero(hero* h, game_state* gs, int keycode);
void handle_key_up_hero(hero* h, int keycode);

#endif