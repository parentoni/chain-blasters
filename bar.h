#ifndef BAR_H
#define BAR_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "config.h"

#define BAR_R_PADDING 638
#define BAR_U_PADDING 34 
#define BAR_PIXELS_PER_LEVEL 212
#define BAR_SEPARATOR_WIDTH 4
#define BAR_HEIGHT 40
#define BAR_INTERNAL_H_PADDING 5

void render_bar(game_state* gs, int bar_sprite_index, solid_state* s, ALLEGRO_FONT* f);
#endif