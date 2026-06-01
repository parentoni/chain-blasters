#include "config.h"

int min(int a, int b) {
	if (a < b) return a;
	return b;
}

int max(int a, int b) {
	if (a >= b) return a;
	return b;
}

float minf(float a, float b) {
	if (a < b) return a;
	return b;
}

float maxf(float a, float b) {
	if (a >= b) return a;
	return b;
}

void initialize_game_state(game_state* gs) {
	gs->points = 0;
	gs->attack_points = 10;
	gs->level = 0;
	gs->tick_count = 0;
	gs->current_fps = FPS;
}

void update_points_game_state(game_state* gs, int points) {
	gs->points += points;
	gs->attack_points += points;
	if(gs->attack_points > MAX_ATTACK_POINTS) gs->attack_points = MAX_ATTACK_POINTS;
}

vec2 random_out_screen() {
	int i = rand() % 4;
	vec2 r;
	if (i == 0) {
		r.x = -ENEMY_W;
		r.y = rand() % SCREEN_H;
	} else if (i == 1) {
		r.x = SCREEN_W + ENEMY_W;
		r.y = rand() % SCREEN_H;
	} else if (i == 2) {
		r.x = rand() % SCREEN_W;
		r.y = -ENEMY_H;
	} else {
		r.x = rand() % SCREEN_W;
		r.y = SCREEN_H + ENEMY_H;
	}

	return r;
}

solid_state load_solid_state() {
	FILE *fptr;	
	fptr = fopen("./static/solid_state.chain", "rb");	

	if (fptr == NULL) {
		// initialize file with solid state
		solid_state a;
		a.high_score = 0;

		fptr =  fopen("./static/solid_state.chain", "wb");
		fwrite(&a, sizeof(solid_state), 1, fptr);	

		fclose(fptr);
		return a;
	}

	solid_state a;
 	fread(&a, sizeof(solid_state), 1, fptr);
 	fclose(fptr);
 	return a;
}

void save_solid_state(solid_state* a) {
	FILE *fptr;	

	fptr = fopen("./static/solid_state.chain", "wb");	
	fwrite(a, sizeof(solid_state), 1, fptr);	

	fclose(fptr);
}
