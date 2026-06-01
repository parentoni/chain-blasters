#include "enemy.h"
#include "sprites.h"
#include "math.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

enemy* enemies[MAX_ENEMIES_COUNT];
wave* waves[MAX_CONCURRENT_WAVES + 1];
bullet* bullets[MAX_BULLETS_COUNT];

int terminator_spawned = 0;

//float f(float x) {
//	return 300 * sin(10* x / SCREEN_W) + SCREEN_H/2;
//}
//
//float fprime(float x) {
//	return 300 * cos(10 * x / SCREEN_W) * 10/SCREEN_W;

void initialize_bullets() {
	for (int i=0;i<MAX_BULLETS_COUNT;i++) bullets[i] = NULL;
}

void create_bullet(hero* h, vec2* pos, game_state* gs) {
	for (int i=0;i<MAX_BULLETS_COUNT;i++) {
		if (bullets[i] != NULL) continue;	

		bullets[i] = malloc(sizeof(bullet));
		bullets[i]->pos = *pos;
		bullets[i]->velocity.x = h->pos.x - pos->x;
		bullets[i]->velocity.y = h->pos.y - pos->y;
		vec2_normalize(&bullets[i]->velocity);

		bullets[i]->velocity.x *= (BULLET_STARTING_SPEED + BULLET_SPEED_PER_LEVEL*gs->level);
		bullets[i]->velocity.y *= (BULLET_STARTING_SPEED + BULLET_SPEED_PER_LEVEL*gs->level);
		break;
	}
}

void initialize_enemies() {
	for (int i=0;i<MAX_ENEMIES_COUNT;i++) {
		enemies[i] = NULL;
	}
}

void initialize_waves() {
	for (int i=0;i<MAX_CONCURRENT_WAVES;i++) {
		waves[i] = NULL;
	}

	// terminator wave
	waves[MAX_CONCURRENT_WAVES] = malloc(sizeof(wave));
	waves[MAX_CONCURRENT_WAVES]->id = 5;
	waves[MAX_CONCURRENT_WAVES]->pattern_type = terminator;
}

void spawn_terminator() {
	for (int i=0;i<MAX_ENEMIES_COUNT;i++) {
		if (enemies[i] != NULL) continue;

		enemies[i] = malloc(sizeof(enemies));
		enemies[i]->pos.x = SCREEN_W/2;
		enemies[i]->pos.y = -ENEMY_H;
		enemies[i]->velocity.x = 0; 
		enemies[i]->velocity.x = 0;
		enemies[i]->tick_count = 0;
		enemies[i]->wave_id = 5;
		break;
	}
}

void update_velocity_enemies(enemy* e, game_state* gs, hero* h) {
	int speed_mult = ENEMY_STARTING_SPEED + ENEMY_SPEED_PER_LEVEL * gs->level;
	switch(waves[e->wave_id]->pattern_type){
		case (kami_kaze):
			e->velocity.x = h->pos.x - e->pos.x;
			e->velocity.y = h->pos.y - e->pos.y;

			vec2_normalize(&(e->velocity));
			e->velocity.x *= speed_mult;
			e->velocity.y *= speed_mult;
			break;
		case (terminator):
			e->velocity.x = h->pos.x - e->pos.x;
			e->velocity.y = h->pos.y - e->pos.y;

			vec2_normalize(&(e->velocity));
			e->velocity.x *= 800; 
			e->velocity.y *= 800;
			break;
		case (straight_line_horizontal):
			if (e->pos.x > SCREEN_W+ENEMY_W && e->velocity.x > 0) {
				e->velocity.x *= -1;
			} else if (e->pos.x < -ENEMY_W && e->velocity.x < 0) {
				e->velocity.x *= -1;
			}
			break;
		default:
			printf("UNK pattern type\n");
			break;
	}
}

int update_state_bullets(hero *h) {
	int lost = 0;
	for (int i=0;i<MAX_BULLETS_COUNT;i++) {
		if (bullets[i] == NULL) continue;

		bullets[i]->pos.x += bullets[i]->velocity.x * 1/FPS;
		bullets[i]->pos.y += bullets[i]->velocity.y * 1/FPS;

		float d = vec2_dist(&h->pos, &bullets[i]->pos);
		if (d < HERO_BOUNDING_BOX_R + BULLET_RADIUS) {
			lost = 1;
		}

		if (bullets[i]->pos.x > SCREEN_W + BULLET_RADIUS || bullets[i]->pos.x < -BULLET_RADIUS) {
			free(bullets[i]); bullets[i] = NULL;
		} else if (bullets[i]->pos.y > SCREEN_H + BULLET_RADIUS || bullets[i]->pos.y < -BULLET_RADIUS) {
			free(bullets[i]); bullets[i] = NULL;
		} 
	}

	return lost;

}

int update_state_enemies(hero* h, game_state* gs) {
	int lost = 0;
	for (int i=0;i<MAX_ENEMIES_COUNT;i++) {
		if (enemies[i] == NULL) continue;

		//update_velocity
 		update_velocity_enemies(enemies[i], gs, h);

		// all velocities are in pixels/second 
		enemies[i]->pos.x += enemies[i]->velocity.x * 1/FPS;
		enemies[i]->pos.y += enemies[i]->velocity.y * 1/FPS;

		enemies[i]->tick_count++;
		if (enemies[i]->tick_count > TIME_TO_TERMINATOR * FPS && !terminator_spawned) {
			terminator_spawned = true;
			spawn_terminator();
		}


		float d = vec2_dist(&(h->pos), &(enemies[i]->pos));
		if ((rand()/(float)RAND_MAX) <= (BULLET_SPAWN_CHANCE)) {
			if (d > BULLET_MIN_DISTANCE) {
				create_bullet(h, &enemies[i]->pos, gs);
			} 
		}

		// check if enemy and hero bound overlap
		if (d < HERO_BOUNDING_BOX_R + ENEMY_BOUNDING_BOX_R) {
			lost = 1;
		}
	}

	return lost; 
}

void render_enemies(int enemy_sprite_index) {
	for (int i=0;i<MAX_ENEMIES_COUNT;i++) {
		if (enemies[i] == NULL) continue;


		ALLEGRO_BITMAP* bmp = get_bitmap_by_index(enemy_sprite_index);
		if (enemies[i]->wave_id == MAX_CONCURRENT_WAVES) {
			bmp = get_bitmap_by_index(0);	
		}
		float angle = atan2(enemies[i]->velocity.y, enemies[i]->velocity.x) + PI/2;

		al_draw_scaled_rotated_bitmap(bmp, ENEMY_ORIGINAL_W/2, ENEMY_ORIGINAL_H/2, enemies[i]->pos.x, enemies[i]->pos.y, ENEMY_W/(float)ENEMY_ORIGINAL_W, ENEMY_H/(float)ENEMY_ORIGINAL_H, angle, 0);

		//debug
		//al_draw_filled_circle(enemies[i]->pos.x, enemies[i]->pos.y, 3, al_map_rgb(0,0,255));
		//al_draw_circle(enemies[i]->pos.x, enemies[i]->pos.y, ENEMY_BOUNDING_BOX_R, al_map_rgb(0, 0, 255), 3);
	}	
}

void render_bullets() {
	for (int i=0;i<MAX_BULLETS_COUNT;i++) {
		if (bullets[i] == NULL) continue;
		al_draw_filled_circle(bullets[i]->pos.x,bullets[i]->pos.y,BULLET_RADIUS,al_map_rgb(255,20,20));
	}

}


enemy* iter_enemies(int enemy_index) {
	if (enemy_index >= MAX_ENEMIES_COUNT) return NULL;
	return enemies[enemy_index];
}


void update_state_wave(game_state* gs, hero* h) {
	int waves_count = 0;
	for (int i=0;i<MAX_CONCURRENT_WAVES;i++) {
		if (waves[i] == NULL) continue;
		if (waves[i]->remaining_enemies == 0) {
			gs->level++;
			free(waves[i]);
			waves[i] = NULL;
			continue;
		}	
		waves_count++;
	}

	int i=0, waves_to_create = min(MAX_CONCURRENT_WAVES, (gs->level/LEVELS_TO_INCREMENT_WAVE_COUNT) + 1) - waves_count;

	while (waves_to_create && i < MAX_CONCURRENT_WAVES) {
		if (waves[i] == NULL) {
			waves[i] = malloc(sizeof(wave));
			waves[i]->id = i;
			waves[i]->remaining_enemies = ENEMIES_PER_WAVE;
			waves[i]->pattern_type = rand() % PATTERN_COUNT;

			int created_enemies = 0, j = 0;
			while (created_enemies < ENEMIES_PER_WAVE && j < MAX_ENEMIES_COUNT) {

				if (enemies[j] == NULL) {

					enemies[j] = malloc(sizeof(enemy));
					enemies[j]->wave_id = i;
					enemies[j]->tick_count = 0;

					switch(waves[i]->pattern_type) {
						case (kami_kaze):
							enemies[j]->pos = random_out_screen();
							break;
						case (straight_line_horizontal):
							if (rand() % 2) {
								enemies[j]->pos.x = -ENEMY_W - 50 * created_enemies;
								enemies[j]->pos.y = h->pos.y;
								enemies[j]->velocity.x = 3*(ENEMY_STARTING_SPEED + ENEMY_SPEED_PER_LEVEL * gs->level);
								enemies[j]->velocity.y = 0;
							} else {
								enemies[j]->pos.x = SCREEN_W + ENEMY_W + 50 * created_enemies;
								enemies[j]->pos.y = h->pos.y;
								enemies[j]->velocity.x = -3 * (ENEMY_STARTING_SPEED + ENEMY_SPEED_PER_LEVEL * gs->level);
								enemies[j]->velocity.y = 0;
							}
							break;
					}
					created_enemies++;
				}
				j++;


			}
			waves_to_create--;

		}
		i++;
	}
}