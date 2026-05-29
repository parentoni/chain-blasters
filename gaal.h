#ifndef GAAL_H
#define GAAL_H

typedef struct {
	float x;
	float y;
} vec2;

float vec2_dist(vec2* a, vec2* b);
float vec2_norm(vec2* a);
void vec2_normalize(vec2* a);

#endif