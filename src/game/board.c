
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "structs.h"
#include "util.h"
#include "board.h"

inline void draw_debug_placement_lines(void);

inline char rgb = 2;

inline Sprite   p_sprite;
inline char     was_pressed_this_frame = 0;
inline SDL_Rect rect = 
{
    0, 0, 160, 160
};
inline SDL_Rect mouse_r  =
{
    0, 0, 1, 1
};


void init_board()
{
    int x, y, margin;
    Vec2i start_offset = {};
    margin = 5;
    start_offset.x = get_scr_width_scaled() / 3;
    start_offset.y = get_scr_height_scaled() / 14;
    SDL_Rect d = {0, 0, 184, 184};

    Entity *tile = NULL;
    for(x = 0; x < GRID_X; x++){
        for(y = 0; y < GRID_Y; y++) 
        {
            tile = &grid_large[x][y];
            tile->active = 1;
            tile->ent_type = GRID_TILE;
            tile->dest.w = 184;
            tile->dest.h = tile->dest.w;

            tile->dest.x = start_offset.x + (x * (tile->dest.w + margin));
            tile->dest.y = start_offset.y + (y * (tile->dest.h + margin));

            tile->x = tile->dest.x;
            tile->y = tile->dest.y;
        }
    }   
}

void update_board(void)
{
    mouse_r.x = game.mouse.x;
    mouse_r.y = game.mouse.y;

    Entity *tile = NULL;
    int x, y;
    for(x = 0; x < GRID_X; x++){
        for(y = 0; y < GRID_Y; y++) 
        {
            tile = &grid_large[x][y];

            if(SDL_HasIntersection(&mouse_r, &tile->dest))
            {
                SDL_Log("COLLISION AT: %i %i", game.mouse.x, game.mouse.y);
                tile->color = G;
            }
            else {
                tile->color = B;
            }
        }
    }
}   

void draw_board(void)
{
    //draw_debug_placement_lines();


    int x, y, margin;
    Vec2i start_offset = {};
    margin = 5;
    start_offset.x = get_scr_width_scaled() / 3;
    start_offset.y = get_scr_height_scaled() / 14;
    SDL_Rect d = {0, 0, 184, 184};
    /*
    for(x = 0; x < 3; x++) {
        for(y = 0; y < 3; y++)
        {
            d.x = start_offset.x + (x * (d.w + margin));
            d.y = start_offset.y + (y * (d.h + margin));
            SDL_RenderFillRect(game.renderer, &d);
        }
    }
    */

    /*
    SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
    start_offset.x = get_scr_width_scaled() / 14;
    start_offset.y = get_scr_height_scaled() / 3;
    d.w = 80;
    d.h = d.w;
    for(x = 0; x < 3; x++) {
        for(y = 0; y < 3; y++)
        {
            d.x = start_offset.x + (x * (d.w + margin));
            d.y = start_offset.y + (y * (d.h + margin));
            if(rgb == 2)
            {
                SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
            }
            else if (rgb == 1)
            {
                SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
            }
            SDL_RenderFillRect(game.renderer, &d);
        }
    }
    */

    Entity *tile = NULL;
    for(x = 0; x < GRID_X; x++){
        for(y = 0; y < GRID_Y; y++) 
        {
            tile = &grid_large[x][y];

            if(tile->color == B)
            {
                SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
            }
            else if (tile->color == G)
            {
                SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
            }
            SDL_RenderFillRect(game.renderer, &tile->dest);
        }
    }
    /*
    blit_from_sheet(game.spritesheet, player.sprite->dest, player.sprite->src, 0, SCREEN_SCALE, 1);

    SDL_Rect d = player.sprite->dest;
    d.w *= SCREEN_SCALE;
    d.h *= SCREEN_SCALE;
    d.x -= (d.w/2);
    d.y -= (d.h/2);
    */

    /*
    SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(game.renderer, &d);
    */
}

inline void draw_debug_placement_lines(void)
{
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        SDL_RenderDrawLine(game.renderer, 
                (i * (get_scr_width_scaled() / 4)),
                0,
                (i * (get_scr_width_scaled() / 4)),
                get_scr_height_scaled());
    }
    for(i = 0; i < 3; i++)
    {
        SDL_RenderDrawLine(game.renderer,
                0,
                (i * (get_scr_height_scaled() / 3)),
                get_scr_width_scaled(),
                (i * (get_scr_height_scaled() / 3)));
    }

}

void reset_board(void)
{
    
}
