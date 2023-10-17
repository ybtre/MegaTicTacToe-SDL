
#include "common.h"

#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "stage.h"
#include "main.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>

Game game;
Entity grid_large[GRID_X][GRID_Y];
Stage stage;
Game_State game_state;

inline void cap_fps(long *THEN, float *REMAINDER);

int main(int argc, char *argv[])
{
    long then;
    float remainder;

    srand(42);

    memset(&game, 0, sizeof(Game));
    
    init_SDL();

    atexit(game_close);

    init_game();

    init_stage();

    then = SDL_GetTicks();
    remainder = 0;

    char running = 1;
    uint32_t total_frame_ticks = 0;
    uint32_t total_frames = 0;
    while(running == 1)
    {
        //start frame timing
        total_frames++;
        uint32_t start_ticks = SDL_GetTicks();
        uint64_t start_perf = SDL_GetPerformanceCounter();

        prepare_scene();

        handle_input();

        game.delegate.logic();
        game.delegate.draw();

        //end frame timing
        uint32_t end_ticks = SDL_GetTicks();
        uint64_t end_perf = SDL_GetPerformanceCounter();
        uint64_t frame_perf = end_perf - start_perf;
        float frame_time = (end_ticks - start_ticks) / 1000.f;
        total_frame_ticks += end_ticks - start_ticks;
        float elapsed_ms = (end_perf - start_perf) / (float)SDL_GetPerformanceFrequency() * 1000.f;


        char fps_buffer[32];
        sprintf(fps_buffer, "Current FPS: %f", (1.f / frame_time));
   
        char avg_buffer[32];
        sprintf(avg_buffer, "Average FPS: %f", 1000.f / (float)total_frame_ticks / total_frames);

        char curr_perf_buffer[32];
        sprintf(curr_perf_buffer, "Current Perf: %f", (float)frame_perf);

        SDL_Rect dest = { 10, 10, 0, 0};
        render_text(fps_buffer, dest, 1.f);
        dest.y += 24;
        render_text(avg_buffer, dest, 1.f);
        dest.y += 24;
        render_text(curr_perf_buffer, dest, 1.f);

        //SDL_Delay(16.666f - elapsed_ms);

        render_scene();
        //cap_fps(&then, &remainder);
    }
    return(0);
}


inline void cap_fps(long *THEN, float *REMAINDER)
{// fps cap logic
    long wait, frame_time;

    wait = 16 + *REMAINDER;

    *REMAINDER -= (int)*REMAINDER;

    frame_time = SDL_GetTicks() - *THEN;

    wait -= frame_time;

    if(wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *REMAINDER += 0.667f;

    *THEN = SDL_GetTicks();
}






















