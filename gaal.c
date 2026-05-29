#include "gaal.h"
#include <math.h>

float vec2_dist(vec2* a, vec2* b) {
	float d1 = (a->x - b->x);
	float d2 = (a->y - b->y);
	return sqrt(d1* d1 + d2 * d2);
}

float vec2_norm(vec2* a) {
	return sqrt(a->x * a->x + a->y * a->y);
}

void vec2_normalize(vec2* a) {
	float n = vec2_norm(a);
	a->x /=  n;
	a->y /=  n;
}