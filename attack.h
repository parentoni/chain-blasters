#ifndef ATTACK_H
#define ATTACK_H

#include "config.h"
#define ATTACK_R_LEN 51
#define SECONDARY_ATTACK_R_LEN 20

typedef struct {
	vec2 center;
	float radius;
	int ttl; // in frames
	int ttbc;
	int is_main;
} attack; // this is the main attack -> aka chain one



int create_main_attack(vec2 pos);

void update_state_attack(game_state* gs);
void render_attacks();

#endif