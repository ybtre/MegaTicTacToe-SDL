
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "structs.h"
#include "util.h"
#include "board.h"

inline void draw_debug_placement_lines(void);
inline char win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT);

inline SDL_Rect mouse_rect  =
{
    0, 0, 1, 1
};

Board board = {};

#define BOARD_X             3
#define BOARD_Y             3
Board boards[BOARD_X][BOARD_Y];


void init_board()
{
    int x, y, gx, gy, margin;
    Vec2i start_offset = {};
    margin = 5;
    start_offset.x = get_scr_width_scaled() / 2 - (55 * 5);
    start_offset.y = get_scr_height_scaled() / 12;
    SDL_Rect d = {0, 0, 184, 184};
    
    Tile tile;
    int tile_size = 184;
    tile_size = 55;

    for(x = 0; x < BOARD_X; x++) {
        for(y = 0; y < BOARD_Y; y++)
        {
            boards[x][y].color = 0;

            boards[x][y].data.x = (start_offset.x + (x * (tile_size * 3 + margin * 3)));
            boards[x][y].data.y = (start_offset.y + (y * (tile_size * 3 + margin * 3)));

            boards[x][y].data.dest.x = boards[x][y].data.x;
            boards[x][y].data.dest.y = boards[x][y].data.y;
            boards[x][y].data.dest.w = tile_size * GRID_X + margin * 2;
            boards[x][y].data.dest.h = tile_size * GRID_Y + margin * 2;
            boards[x][y].data.idx = x;
            boards[x][y].data.idy = y;

            for(gx = 0; gx < GRID_X; gx++){
                for(gy = 0; gy < GRID_Y; gy++) 
                {
                    tile.state = TILE_EMPTY;
                    
                    tile.data.dest.w = tile_size;
                    tile.data.dest.h = tile.data.dest.w;

                    tile.data.dest.x = boards[x][y].data.x + (gx * (tile.data.dest.w + margin));
                    tile.data.dest.y = boards[x][y].data.y + (gy * (tile.data.dest.h + margin));

                    tile.data.x = tile.data.dest.x;
                    tile.data.y = tile.data.dest.y;

                    tile.data.idx = x;
                    tile.data.idy = y;

                    boards[x][y].tiles[gx][gy] = tile;
                }
            }   
        }
    }

    stage.current_board = &boards[1][1];
}

void update_board(void)
{
    Board *board = NULL;
    int x, y, gx, gy;
    /*
    for(x = 0; x < BOARD_X; x++) {
        for(y = 0; y < BOARD_Y; y++)
        {
        */
            //board = &boards[x][y];
            board = stage.current_board;
            mouse_rect.x = game.mouse.x;
            mouse_rect.y = game.mouse.y;

            Tile *tile = NULL;
            for(gx = 0; gx < GRID_X; gx++) {
                for(gy = 0; gy < GRID_Y; gy++) 
                {
                    tile = &board->tiles[gx][gy];

                    char hover = 0;
                    char pressed = 0;
                    if(SDL_HasIntersection(&mouse_rect, &tile->data.dest))
                    {

                        hover = 1;

                        if(game.mouse.button[SDL_BUTTON_LEFT])
                        {
                            pressed = 1;
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
                        if(stage.turn == TURN_GREEN)
                        {
                            tile->state = TILE_GREEN;

                            // hand over turn to other playuer
                            stage.turn = TURN_ORANGE;
                            stage.prev_turn = TURN_GREEN;
                        }
                        elif(stage.turn == TURN_ORANGE)
                        {
                            tile->state = TILE_ORANGE;

                            // hand over turn to other playuer
                            stage.turn = TURN_GREEN;
                            stage.prev_turn = TURN_ORANGE;
                        }

                        stage.current_board = &boards[gx][gy];
                    }
                }
            }

            {//check win condition for current small board
                if(board->winner == 0)
                {
                    char win = false;
                    Tile *mid, *left, *right = NULL;
                    int i = 0;
                    for(i = 0; i < 3; i++)
                    {
                        mid = &board->tiles[1][i];
                        left = &board->tiles[0][i];
                        right = &board->tiles[2][i];

                        win = win_state_check(mid, left, right);
                        if(win)
                        {
                            board->winner = stage.prev_turn;
                            break;
                        }

                        mid = &board->tiles[i][1];
                        left = &board->tiles[i][0];
                        right = &board->tiles[i][2];

                        win = win_state_check(mid, left, right);
                        if(win)
                        {
                            board->winner = stage.prev_turn; 
                            break;
                        }
                    }

                    mid = &board->tiles[1][1];
                    left = &board->tiles[0][0];
                    right = &board->tiles[2][2];

                    win = win_state_check(mid, left, right);
                    if(win)
                        board->winner = stage.prev_turn; 

                    if(!win)
                    {
                        mid = &board->tiles[1][1];
                        left = &board->tiles[0][2];
                        right = &board->tiles[2][0];

                        win = win_state_check(mid, left, right);
                        if(win)
                            board->winner = stage.prev_turn; 
                    }
                }
            }
            /*
        }
    }   
*/
}

inline char win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT)
{
    char win = false;

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

        win = true;
    }

    return(win);
}

void draw_board(void)
{
    int x, y, gx, gy;
    Tile *tile = NULL;
    Board *b = NULL;
    for(x = 0; x < BOARD_X; x++) {
        for(y = 0; y < BOARD_Y; y++)
        {
            b = &boards[x][y];
            SDL_SetRenderDrawColor(game.renderer, 89, 102, 102, 255);
            SDL_RenderFillRect(game.renderer, &b->data.dest);

            for(gx = 0; gx < GRID_X; gx++){
                for(gy = 0; gy < GRID_Y; gy++) 
                {
                    tile = &b->tiles[gx][gy];

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

                    if(b->winner == PLAYER_GREEN)
                    {
                        SDL_SetRenderDrawColor(game.renderer, 190, 215, 134, 255);
                        SDL_Rect r = { 10, 300, 100, 100 };
                        SDL_RenderFillRect(game.renderer, &r);
                    }
                    if(b->winner == PLAYER_ORANGE)
                    {
                        SDL_SetRenderDrawColor(game.renderer, 235, 162, 84, 255);
                        SDL_Rect r = { 10, 420, 100, 100 };
                        SDL_RenderFillRect(game.renderer, &r);
                    }
                }
            }
        }
    }
}

inline void draw_debug_placement_lines(void)
{
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);

    int r = 9;
    int c = 16;
    int i = 0;
    for(i = 0; i < c; i++)
    {
        SDL_RenderDrawLine(game.renderer, 
                (i * (get_scr_width_scaled() / c)),
                0,
                (i * (get_scr_width_scaled() / c)),
                get_scr_height_scaled());
    }
    for(i = 0; i < r; i++)
    {
        SDL_RenderDrawLine(game.renderer,
                0,
                (i * (get_scr_height_scaled() / r)),
                get_scr_width_scaled(),
                (i * (get_scr_height_scaled() / r)));
    }

}

void reset_board(void)
{
    
}
