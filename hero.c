#include "hero.h"
#include "sprites.h"
#include "attack.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

void initialize_hero(hero* h) {
	h->pos.x = SCREEN_W/2;
	h->pos.y = SCREEN_H/2;
	h->velocity.x = 0;
	h->velocity.y = 0;
}

void update_state_hero(hero* h) {
	h->pos.x += h->velocity.x * HERO_SPEED / FPS;
	h->pos.y += h->velocity.y * HERO_SPEED / FPS;

	// bounds
	if (h->pos.x > SCREEN_W - HERO_W/2.0) h->pos.x = SCREEN_W - HERO_W/2.0;
	if (h->pos.x < HERO_W/2.0) h->pos.x = HERO_W/2.0;
	if (h->pos.y < HERO_H/2.0) h->pos.y = HERO_H/2.0;
	if (h->pos.y > SCREEN_H - HERO_H/2.0) h->pos.y = SCREEN_H - HERO_H/2.0;

	
}

void render_hero(hero* h, int hero_bitmap_index) {	
	al_draw_scaled_bitmap(get_bitmap_by_index(hero_bitmap_index), 0, 0, HERO_ORIGINAL_W, HERO_ORIGINAL_H, h->pos.x - HERO_W/2.0, h->pos.y - HERO_H/2.0, HERO_W, HERO_H, 0);

	//debug
	//al_draw_filled_circle(h->pos.x, h->pos.y, BULLET_MIN_DISTANCE, al_map_rgb(255,0,0));
	//al_draw_circle(h->pos.x, h->pos.y, BULLET_MIN_DISTANCE, al_map_rgb(255,0, 0), 3);
}

void handle_key_down_hero(hero* h, game_state* gs, int keycode) {
	if (keycode == ALLEGRO_KEY_W || keycode == ALLEGRO_KEY_UP) {
		h->velocity.y -= 1;
	} else if (keycode == ALLEGRO_KEY_S || keycode == ALLEGRO_KEY_DOWN) {
		h->velocity.y += 1;
	} else if (keycode == ALLEGRO_KEY_D || keycode == ALLEGRO_KEY_RIGHT) {
		h->velocity.x += 1;
	} else if (keycode == ALLEGRO_KEY_A || keycode == ALLEGRO_KEY_LEFT) {
		h->velocity.x -= 1;
	} else if (keycode == ALLEGRO_KEY_SPACE) {
		if (gs->attack_points < ATTACK_POINTS_COST) return;

		int success = create_main_attack(h->pos);
		if (success) gs->attack_points -= ATTACK_POINTS_COST;
	}
}

void handle_key_up_hero(hero* h, int keycode) {
	if (keycode == ALLEGRO_KEY_W || keycode == ALLEGRO_KEY_UP) {
		h->velocity.y += 1;
	} else if (keycode == ALLEGRO_KEY_S || keycode == ALLEGRO_KEY_DOWN) {
		h->velocity.y -= 1;
	} else if (keycode == ALLEGRO_KEY_D || keycode == ALLEGRO_KEY_RIGHT) {
		h->velocity.x -= 1;
	} else if (keycode == ALLEGRO_KEY_A || keycode == ALLEGRO_KEY_LEFT) {
		h->velocity.x += 1;
	}
}