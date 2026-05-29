#include "config.h"
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP *BITMAP_REGISTER[MAX_SPRITES]; 

void intialize_sprites(char* files[], int file_count) {
	if (file_count > MAX_SPRITES) {
		perror("Amount of files loaded bigger than MAX_SPRITES");
		exit(1);
	}

	for (int i=0;i<file_count;i++) {
		BITMAP_REGISTER[i] = al_load_bitmap(files[i]); 	
		if (!BITMAP_REGISTER[i]) {
			perror("Couldn't load given image.");	
			exit(1);
		}
	}
}

ALLEGRO_BITMAP* get_bitmap_by_index(int idx) {
	return BITMAP_REGISTER[idx];
}
