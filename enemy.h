#ifndef ENEMY_HE
#define ENEMY_HE

#include "config.h"		
#include "hero.h"

#define PATTERN_COUNT 2 // n - 1, the rng shouldn't be able to spawn terminator

enum PATTERN {
	kami_kaze,
	straight_line_horizontal,
	terminator
};

typedef struct {
	vec2 pos; 
	vec2 velocity;
} bullet;

typedef struct {
	vec2 pos;
	vec2 velocity;
	int wave_id;
	int tick_count;
} enemy;

typedef struct {
	int remaining_enemies;	
	int id;
	enum PATTERN pattern_type;
} wave;

extern wave* waves[MAX_CONCURRENT_WAVES + 1];// terminator wave-> +1
extern enemy* enemies[MAX_ENEMIES_COUNT];

void initialize_enemies();
void initialize_waves();
void initialize_bullets();

// return true if game lost
int update_state_enemies(hero* h, game_state* gs);
int update_state_bullets(hero* h);
void update_state_wave(game_state* gs, hero* h);

void render_enemies(int enemy_sprite_index);
void render_bullets();

enemy* iter_enemies(int enemy_index);

#endif