#ifndef CONSTANTS
#define CONSTANTS

#include <stdlib.h>
#include <allegro5/allegro.h>
#include "gaal.h"

// math
#define PI 3.14159265358979323846

// screen 
#define FPS 60
#define SCREEN_W 1920
#define SCREEN_H 1080
#define MAX_SPRITES 10

// hero
#define HERO_W 100 
#define HERO_H 108
#define HERO_ORIGINAL_W 125
#define HERO_ORIGINAL_H 135
#define HERO_SPEED 750 // pixels/second
#define HERO_BOUNDING_BOX_R 50

//attack
#define MAX_ATTACK_POINTS 30
#define ATTACK_POINTS_COST 10

#define MAIN_ATTACK_RADIUS 150 
#define MAIN_ATTACK_TTL (1.5 * FPS)
#define CHAIN_ATTACK_RADIUS 100 
#define CHAIN_ATTACK_TTL (1 * FPS) // change to 1 * FPS

// enemies
#define MAX_ENEMIES_COUNT 50 
#define ENEMY_ORIGINAL_W 182
#define ENEMY_ORIGINAL_H 232
#define ENEMY_BOUNDING_BOX_R 35
#define ENEMY_W 70
#define ENEMY_H 80
#define POINTS_PER_ENEMY 2
#define ENEMY_STARTING_SPEED 200 // 
#define ENEMY_SPEED_PER_LEVEL 10

// bullet 
#define MAX_BULLETS_COUNT 75
#define BULLET_STARTING_SPEED 500
#define BULLET_SPEED_PER_LEVEL 30
#define BULLET_RADIUS 10
#define BULLET_SPAWN_CHANCE 0.001 // 1 in 100, rand() % 100 < 1;
#define BULLET_MIN_DISTANCE 300

// waves
#define MAX_CONCURRENT_WAVES 5 
#define LEVELS_TO_INCREMENT_WAVE_COUNT 5 // a cada X levels + 1 wave concorrente;
#define ENEMIES_PER_WAVE 10

// General game definitions
#define POINTS_PER_SECOND 1 
#define TIME_TO_TERMINATOR 15 //seconds

typedef struct {
	int points;
	int attack_points; // gain points with points, but it's capped to 30, each attack costs 10;
	int level;
	int tick_count;
	int current_fps;
} game_state;

typedef struct {
	int high_score;
} solid_state;

void initialize_game_state(game_state* gs);
void update_points_game_state(game_state* gs, int points);
vec2 random_out_screen();

solid_state load_solid_state();
void save_solid_state(solid_state* a);

int min(int a, int b);
int max(int  a, int b);
float minf(float a, float b);
float maxf(float a, float b);

#endif