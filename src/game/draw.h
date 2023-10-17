#ifndef DRAW_H
#define DRAW_H

#include "../Include/SDL2/SDL.h"

void prepare_scene(void);
void render_scene(void);

SDL_Texture* load_texture(const char* PATH);
void blit(SDL_Texture* TEXTURE, const int X, const int Y, const float SCALE, const int CENTER);
void blit_rect(SDL_Texture* TEXTURE, SDL_Rect* SRC, int X, int Y, float SCALE);
void blit_from_sheet(SDL_Texture* TEXTURE, SDL_Rect DEST, SDL_Rect SRC, const float ANGLE, const float SCALE, const int CENTER);
void blit_rotated(SDL_Texture *TEXTURE, const int X, const int Y, const float ANGLE, const float SCALE, const int CENTER);

#endif
