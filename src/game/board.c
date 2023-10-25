
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "structs.h"
#include "util.h"
#include "board.h"

inline void draw_debug_placement_lines(void);
inline char win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT);
inline char win_game_check(Board *MID, Board *LEFT, Board *RIGHT);

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

            if(tile->state == TILE_WIN_GREEN OR tile->state == TILE_WIN_ORANGE)
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

    for(gx = 0; gx < GRID_X; gx++) {
        for(gy = 0; gy < GRID_Y; gy++) 
        {
            tile = &stage.current_board->tiles[gx][gy];
            
            if(tile->state == TILE_EMPTY)
            {
                tile->state = TILE_ACTIVE_BOARD;
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

    {//check win condition for full board
        Board mid_board;
        Board left_board;
        Board right_board;

        for(int i = 0; i < 3; i++)
        {
            mid_board = boards[i][1];
            left_board = boards[i][0];
            right_board = boards[i][2];

            win_game_check(&mid_board, &left_board, &right_board);

            if(stage.game_winner != 0)
                break;

            mid_board = boards[1][i];
            left_board = boards[0][i];
            right_board = boards[2][i];

            win_game_check(&mid_board, &left_board, &right_board);

            if(stage.game_winner != 0)
                break;
        }

        if(stage.game_winner != 0)
        {
            mid_board = boards[1][1];
            left_board = boards[0][0];
            right_board = boards[2][2];
            
            win_game_check(&mid_board, &left_board, &right_board);

            if(stage.game_winner != 0)
            {
                mid_board = boards[1][1];
                left_board = boards[0][2];
                right_board = boards[2][0];

                win_game_check(&mid_board, &left_board, &right_board);
            }
        }
    }
    
    if(stage.game_winner != 0)
    {
       game_state = GAME_OVER; 
    }
}

inline char win_game_check(Board *MID, Board *LEFT, Board *RIGHT)
{

    if(MID->winner == PLAYER_GREEN
       AND LEFT->winner == PLAYER_GREEN
       AND RIGHT->winner == PLAYER_GREEN)
    {
        stage.game_winner = PLAYER_GREEN;
    }

    if(MID->winner == PLAYER_ORANGE
       AND LEFT->winner == PLAYER_ORANGE
       AND RIGHT->winner == PLAYER_ORANGE)
    {
        stage.game_winner = PLAYER_ORANGE;
    }

    return(stage.game_winner);
}

inline char win_state_check(Tile *MID, Tile *LEFT, Tile *RIGHT)
{
    char win = false;

    if( MID->state == TILE_GREEN
        AND LEFT->state == TILE_GREEN
        AND RIGHT->state == TILE_GREEN)
    {
        MID->state = TILE_WIN_GREEN;
        LEFT->state = TILE_WIN_GREEN;
        RIGHT->state = TILE_WIN_GREEN;
        win = true;
    }

    if( MID->state == TILE_ORANGE
        AND LEFT->state == TILE_ORANGE
        AND RIGHT->state == TILE_ORANGE)
    {
        MID->state = TILE_WIN_ORANGE;
        LEFT->state = TILE_WIN_ORANGE;
        RIGHT->state = TILE_WIN_ORANGE;

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
                    elif(tile->state == TILE_WIN_GREEN)
                    {
                        SDL_SetRenderDrawColor(game.renderer, 26, 72, 44, 255);
                    }
                    elif(tile->state == TILE_WIN_ORANGE)
                    {
                        SDL_SetRenderDrawColor(game.renderer, 143, 97, 51, 255);
                    }
                    elif(tile->state == TILE_ACTIVE_BOARD)
                    {
                        SDL_SetRenderDrawColor(game.renderer, 51, 73, 76, 255);
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
    stage.game_winner = 0;
    stage.turn = TURN_GREEN;
    stage.prev_turn = TURN_NOONE;
    stage.current_board = &boards[1][1];

    int bx, by, tx, ty = 0;
    Board *board = NULL;
    Tile *tile = NULL;

    for(bx = 0; bx < BOARD_X; bx++){
        for(by = 0; by < BOARD_Y; by++)
        {
            board = &boards[bx][by];

            board->winner = 0;

            for(tx = 0; tx < GRID_X; tx++){
                for(ty = 0; ty < GRID_Y; ty++)
                {
                    tile = &board->tiles[tx][ty];

                    tile->state = TILE_EMPTY;
                }
            }
        }
    }
}
