
#include "common.h"

#include "defs.h"
#include "draw.h"
#include "structs.h"
#include <string.h>

inline SDL_Texture* get_texture_from_cache(const char *NAME);
inline void add_texture_to_cache(const char *NAME, SDL_Texture *SDLTEXTURE);

void prepare_scene(void)
{
	//SDL_SetRenderDrawColor(game.renderer, 190, 215, 134, 255);
	SDL_SetRenderDrawColor(game.renderer, 16, 33, 43, 255);
	SDL_RenderClear(game.renderer);
}

void render_scene(void)
{
	SDL_RenderPresent(game.renderer);
}

SDL_Texture* load_texture(const char* PATH)
{
    SDL_Texture *texture;

    texture = get_texture_from_cache(PATH);

    ifnull(texture)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", PATH);
        texture = IMG_LoadTexture(game.renderer, PATH);

        add_texture_to_cache(PATH, texture);
    }

    return(texture);
};

void blit(SDL_Texture* TEXTURE, const int X, const int Y, const float SCALE, const int CENTER)
{
    SDL_Rect dest;

    dest.x = X;
    dest.y = Y;
    //query texture params are OUT params
    SDL_QueryTexture(TEXTURE, NULL, NULL, &dest.w, &dest.h);

    dest.w *= SCALE;
    dest.h *= SCALE;

    if(CENTER)
    {
        dest.x -= dest.w / 2;
        dest.y -= dest.h / 2;
    }

    SDL_RenderCopy(game.renderer, TEXTURE, NULL, &dest);

#ifdef DRAW_RECTS
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(game.renderer, &dest);
#endif  
};

void blit_from_sheet(SDL_Texture* TEXTURE, SDL_Rect DEST, SDL_Rect SRC, const float ANGLE, const float SCALE, const int CENTER)
{
    DEST.w = SRC.w * SCALE;
    DEST.h = SRC.h * SCALE;

//#define TEST
#ifdef TEST
    //S is temp to zoom out a bit
    float s = SCALE / 4;
    DEST.w = SRC.w * s;
    DEST.h = SRC.h * s;
#endif

    if(CENTER)
    {
        DEST.x -= DEST.w / 2;
        DEST.y -= DEST.h / 2;
    }
    
    SDL_RenderCopyEx(game.renderer, TEXTURE, &SRC, &DEST, ANGLE, NULL, SDL_FLIP_NONE);

//#define DRAW_RECTS
#ifdef DRAW_RECTS
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(game.renderer, &DEST);
#endif  
};

void blit_rect(SDL_Texture* TEXTURE, SDL_Rect* SRC, int X, int Y, float SCALE)
{
    SDL_Rect dest;

    dest.x = X;
    dest.y = Y;
    dest.w = SRC->w * SCALE;
    dest.h = SRC->h * SCALE;

    SDL_RenderCopy(game.renderer, TEXTURE, SRC, &dest);

#ifdef DRAW_RECTS
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(game.renderer, &dest);
#endif  
};


void blit_rotated(SDL_Texture *TEXTURE, const int X, const int Y, const float ANGLE, const float SCALE, const int CENTER)
{
    SDL_Rect dest;

    dest.x = X;
    dest.y = Y;
    SDL_QueryTexture(TEXTURE, NULL, NULL, &dest.w, &dest.h);
    dest.w *= SCALE;
    dest.h *= SCALE;

    if(CENTER)
    {
        dest.x -= (dest.w / 2);
        dest.y -= (dest.h / 2);
    }

    SDL_RenderCopyEx(game.renderer, TEXTURE, NULL, &dest, ANGLE, NULL, SDL_FLIP_NONE);

#ifdef DRAW_RECTS
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(game.renderer, &dest);
#endif  
}

inline SDL_Texture* get_texture_from_cache(const char *NAME)
{
    SDL_Texture* result = NULL;
    
    int i;
    for(i = 0; i < TEXTURE_CACHE_MAX; i++)
    {
        if(strcmp(game.texture_cache[i].name, NAME) == 0)
        {
            result = game.texture_cache[i].texture;
            break;
        }
    }

    return(result);
}

inline void add_texture_to_cache(const char *NAME, SDL_Texture *SDLTEXTURE)
{
    Texture texture = {};
    
    int i;
    for(i = 0; i < TEXTURE_CACHE_MAX; i++)
    {
        if(game.texture_cache[i].active == 0)
        {
            STRNCPY(game.texture_cache[i].name, NAME, MAX_NAME_LENGTH);
            game.texture_cache[i].texture = SDLTEXTURE;

            game.texture_cache[i].active = 1;
            break;
        }
    }

}
