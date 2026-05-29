#include "attack.h"
#include "enemy.h"
#include <allegro5/allegro_primitives.h>

int is_main_active = 0;
attack* attacks[ATTACK_R_LEN] = {NULL}; // array of possible attacks, the chain combination counts as an attack

int create_main_attack(vec2 pos) {
	if (is_main_active) return 0;


	for (int i=0;i<ATTACK_R_LEN;i++) {
		if (attacks[i] != NULL) continue;
		attacks[i] = malloc(sizeof(attack));	
		attacks[i]->center = pos;
		attacks[i]->radius = MAIN_ATTACK_RADIUS;
		attacks[i]->is_main = 1;
		attacks[i]->ttl = MAIN_ATTACK_TTL;
		attacks[i]->ttbc = 15;
		is_main_active = 1;
		return 1;
	}

	return 0;
}

void create_chain_attack(vec2 pos) {
	for (int i=0;i<ATTACK_R_LEN;i++) {
		if (attacks[i] != NULL) continue;
		attacks[i] = malloc(sizeof(attack));
		attacks[i]->center = pos;
		attacks[i]->radius = CHAIN_ATTACK_RADIUS;
		attacks[i]->is_main = 0;
		attacks[i]->ttl = CHAIN_ATTACK_TTL;
		attacks[i]->ttbc = 5;
		return;
	}

}

void update_state_attack(game_state* gs) {
	// terrible approach
	for (int i=0;i<ATTACK_R_LEN;i++) {
		if (attacks[i] == NULL) continue;

		//attack not created yet
		if (attacks[i]->ttbc > 0) {
			attacks[i]->ttbc--;
			continue;
		}

		attacks[i]->ttl--;
		if (attacks[i]->ttl > 0) {
			// check if attack kills any enemy
			for (int j=0;j<MAX_ENEMIES_COUNT;j++) {
				if (enemies[j] == NULL) continue;

				float d = vec2_dist(&(enemies[j]->pos), &(attacks[i]->center));
				if (d < ENEMY_BOUNDING_BOX_R + attacks[i]->radius) {
					if (enemies[j]->wave_id == MAX_CONCURRENT_WAVES) continue; // terminator

					create_chain_attack(enemies[j]->pos);
					update_points_game_state(gs, POINTS_PER_ENEMY);

					waves[enemies[j]->wave_id]->remaining_enemies--;

					free(enemies[j]);
					enemies[j] = NULL;
				}
			}

		} else{
			// o ataque não está mais ativo
			// delete now
			if (attacks[i]->is_main) is_main_active = 0;
			free(attacks[i]);
			attacks[i] = NULL;
		}
	}
}

void render_attacks() {
    for (int i = 0; i < ATTACK_R_LEN; i++) {
        if (attacks[i] == NULL) continue;

        int thick = attacks[i]->is_main ? 6 : 3;

        if (attacks[i]->ttbc > 0) {
            int max_ttbc = 15;
            if (!attacks[i]->is_main) max_ttbc = 5;

            if (attacks[i]->ttbc > max_ttbc) continue;
            float progress = 1.0 - (float)attacks[i]->ttbc / max_ttbc; // 0.0 → 1.0
            float anim_radius = attacks[i]->radius * progress;
            al_draw_circle(attacks[i]->center.x, attacks[i]->center.y, anim_radius, al_map_rgb(255, 255, 255), thick);
        } else {
            al_draw_circle(attacks[i]->center.x, attacks[i]->center.y, attacks[i]->radius, al_map_rgb(100, 100, 220), thick);
        }
    }
}

//void render_attacks() {
//	for (int i=0;i<ATTACK_R_LEN;i++) {
//		if (attacks[i] == NULL) continue;
//		if (attacks[i]->ttbc > 0) continue;
//
//		int thick = 3;
//		if (attacks[i]->is_main) thick = 6;
//
//		al_draw_circle(attacks[i]->center.x, attacks[i]->center.y, attacks[i]->radius, al_map_rgb(100, 100, 220), thick);
//	}
//
//}