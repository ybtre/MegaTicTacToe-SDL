
#include "common.h"

#include "defs.h"
#include "init.h"
#include "text.h"

void init_game(void)
{
    init_fonts();
    //should init game.spritesheet here
}

void init_SDL(void)
{
	int renderer_flags, window_flags;

	renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	window_flags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Couldn't initialize SDL Mixer\n");
		exit(1);
	}

    if (TTF_Init() < 0) 
    {
		printf("Couldn't initialize SDL TTF: %s\n", TTF_GetError());
        exit(1);
    }

    game.font = TTF_OpenFont("fonts/font.ttf", 24);
        if (!game.font) {
            SDL_Log(TTF_GetError());
            exit(1);
        }

	Mix_AllocateChannels(MAX_SND_CHANNELS);

	game.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, window_flags);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // 0 for pixel art

	game.renderer = SDL_CreateRenderer(game.window, -1, renderer_flags);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	//SDL_ShowCursor(0);
}

void game_close(void)
{
	SDL_DestroyRenderer(game.renderer);

	SDL_DestroyWindow(game.window);

	SDL_Quit();
}
