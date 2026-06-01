#ifndef SPRITES
#define SPRITES

// load sprites and register it to sprite register;
void initialize_sprites(char* files[], int file_count);
void initialize_audio();
ALLEGRO_BITMAP* get_bitmap_by_index(int idx);

#endif