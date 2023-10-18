
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "structs.h"
#include "util.h"
#include "board.h"

inline void draw_debug_placement_lines(void);
inline void win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT);

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

Board board = {};


void init_board()
{
    int x, y, margin;
    Vec2i start_offset = {};
    margin = 5;
    start_offset.x = get_scr_width_scaled() / 3;
    start_offset.y = get_scr_height_scaled() / 14;
    SDL_Rect d = {0, 0, 184, 184};

    board.color = 0;

    board.data.x = 0;
    board.data.y = 0;
    board.data.dest.x = 0;
    board.data.dest.y = 0;
    board.data.dest.w = 400;
    board.data.dest.h = 400;
    board.data.idx = 0;
    board.data.idy = 0;

    Tile tile;
    for(x = 0; x < GRID_X; x++){
        for(y = 0; y < GRID_Y; y++) 
        {
            tile.state = TILE_EMPTY;
            
            tile.data.dest.w = 184;
            tile.data.dest.h = tile.data.dest.w;

            tile.data.dest.x = start_offset.x + (x * (tile.data.dest.w + margin));
            tile.data.dest.y = start_offset.y + (y * (tile.data.dest.h + margin));

            tile.data.x = tile.data.dest.x;
            tile.data.y = tile.data.dest.y;

            tile.data.idx = x;
            tile.data.idy = y;

            board.tiles[x][y] = tile;
        }
    }   
}

void update_board(void)
{
    mouse_r.x = game.mouse.x;
    mouse_r.y = game.mouse.y;

    Tile *tile = NULL;
    int x, y;
    for(x = 0; x < GRID_X; x++) {
        for(y = 0; y < GRID_Y; y++) 
        {
            tile = &board.tiles[x][y];

            char hover = 0;
            char pressed = 0;
            if(SDL_HasIntersection(&mouse_r, &tile->data.dest))
            {

                hover = 1;

                if(game.mouse.button[SDL_BUTTON_LEFT])
                {
                    pressed = 1;
                }
                if(game.mouse.button[SDL_BUTTON_RIGHT])
                {
                    pressed = 2;
                }
            }
            else 
            {
                hover = 0;
                pressed = 0;
            }

            if(tile->state == TILE_WIN)
                continue;

            if(hover == 1 AND pressed == 0 
                    AND tile->state != TILE_GREEN AND tile->state != TILE_ORANGE)
            {
                tile->state = TILE_HIGHLIGHTED;
            }
            elif(hover == 0 AND pressed == 0 
                    AND tile->state != TILE_GREEN AND tile->state != TILE_ORANGE)
            {
                tile->state = TILE_EMPTY;
            }
            elif(hover == 1 AND pressed == 1 AND tile->state == TILE_HIGHLIGHTED)
            {
                tile->state = TILE_GREEN;
            }
            elif(hover == 1 AND pressed == 2 AND tile->state == TILE_HIGHLIGHTED)
            {
                tile->state = TILE_ORANGE;
            }
        }
    }

    //check win condition
    Tile *mid, *left, *right = NULL;
    int i = 0;
    for(i = 0; i < 3; i++)
    {
        mid = &board.tiles[1][i];
        left = &board.tiles[0][i];
        right = &board.tiles[2][i];

        win_state_check(mid, left, right);

        mid = &board.tiles[i][1];
        left = &board.tiles[i][0];
        right = &board.tiles[i][2];

        win_state_check(mid, left, right);
    }

    mid = &board.tiles[1][1];
    left = &board.tiles[0][0];
    right = &board.tiles[2][2];

    win_state_check(mid, left, right);

    mid = &board.tiles[1][1];
    left = &board.tiles[0][2];
    right = &board.tiles[2][0];

    win_state_check(mid, left, right);
}   

inline void win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT)
{
    if(     (MID->state == TILE_GREEN
            AND LEFT->state == TILE_GREEN
            AND RIGHT->state == TILE_GREEN)
        OR
            (MID->state == TILE_ORANGE
            AND LEFT->state == TILE_ORANGE
            AND RIGHT->state == TILE_ORANGE))
    {
        MID->state = TILE_WIN;
        LEFT->state = TILE_WIN;
        RIGHT->state = TILE_WIN;
    }
}

void draw_board(void)
{
    //draw_debug_placement_lines();
    

    int x, y;
    Tile *tile = NULL;
    for(x = 0; x < GRID_X; x++){
        for(y = 0; y < GRID_Y; y++) 
        {
            tile = &board.tiles[x][y];
            
            SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
            if(tile->state == TILE_GREEN)
            {
                SDL_SetRenderDrawColor(game.renderer, 190, 215, 134, 255);
            }
            elif(tile->state == TILE_ORANGE)
            {
                SDL_SetRenderDrawColor(game.renderer, 235, 162, 84, 255);
            }
            elif(tile->state == TILE_EMPTY)
            {
                SDL_SetRenderDrawColor(game.renderer, 24, 50, 62, 255);
            }
            elif(tile->state == TILE_HIGHLIGHTED)
            {
                SDL_SetRenderDrawColor(game.renderer, 89, 102, 102, 255);
            }
            elif(tile->state == TILE_WIN)
            {
                SDL_SetRenderDrawColor(game.renderer, 255, 102, 102, 255);
            }

            SDL_RenderFillRect(game.renderer, &tile->data.dest);

        }
    }
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
