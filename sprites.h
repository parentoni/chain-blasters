#ifndef SPRITES
#define SPRITES

ALLEGRO_BITMAP *BITMAP_REGISTER[MAX_SPRITES]; 
// load sprites and register it to sprite register;
void intialize_sprites(char* files[], int file_count);
ALLEGRO_BITMAP* get_bitmap_by_index(int idx);

#endif