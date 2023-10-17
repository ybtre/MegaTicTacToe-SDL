
#include "common.h"

#include "defs.h"
#include "input.h"

static void handle_key_down(SDL_KeyboardEvent *EVENT)
{
    if(EVENT->repeat == 0)
    {
        if(EVENT->keysym.scancode < MAX_KEYBOARD_KEYS)
        {
            game.keyboard[EVENT->keysym.scancode] = 1;
        }
    }
};

static void handle_key_up(SDL_KeyboardEvent *EVENT)
{
    if(EVENT->repeat == 0)
    {
        if(EVENT->keysym.scancode < MAX_KEYBOARD_KEYS)
        {
            game.keyboard[EVENT->keysym.scancode] = 0;
        }
    }
};

inline void handle_mouse_up(SDL_MouseButtonEvent *E)
{
    game.mouse.button[E->button] = 0;
}

inline void handle_mouse_down(SDL_MouseButtonEvent *E)
{
    game.mouse.button[E->button] = 1;
}

void handle_input(void)
{
	SDL_Event event;

    //memset(game.input_text, '\0', MAX_LINE_LENGTH);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
                exit(0);
				break;

            case SDL_KEYDOWN:
                handle_key_down(&event.key);
                break;

            case SDL_KEYUP:
                handle_key_up(&event.key);
                break;

            case SDL_MOUSEBUTTONUP:
                handle_mouse_up(&event.button);
                break;

            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_down(&event.button);
                break;

            case SDL_MOUSEWHEEL:
                game.mouse.wheel = event.wheel.y;
                break;

            //case SDL_TEXTINPUT:
                //STRNCPY(game.input_text, event.text.text, MAX_LINE_LENGTH);

			default:
				break;
		}
	}

    SDL_GetMouseState(&game.mouse.x, &game.mouse.y);
}
