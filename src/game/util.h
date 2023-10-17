#ifndef UTIL_H
#define UTIL_H

#include "common.h"

void render_text(char* text, SDL_Rect dest, float SCALE);
void calc_slope(int X1, int Y1, int X2, int Y2, float *DX, float *DY);
float get_angle(int X1, int Y1, int X2, int Y2);
int get_scr_width_scaled(void);
int get_scr_height_scaled(void);

typedef struct Vec2i
{
    int x;
    int y;
} Vec2i;

extern void* mmalloc(uint64_t SIZE);
extern void mfree(void* MEM_TO_FREE);

#endif
