
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "text.h"
#include <string.h>

inline SDL_Texture *font_texture;
inline char         draw_text_buffer[MAX_LINE_LENGTH];

void init_fonts()
{
    font_texture = load_texture("assets/font.png");
};

void draw_text(int X, int Y, int R, int G, int B, int ALIGN, const char *FORMAT, ...)
{
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&draw_text_buffer, '\0', sizeof(draw_text_buffer));

    va_start(args, FORMAT);
    vsprintf(draw_text_buffer, FORMAT, args);
    va_end(args);

    len = strlen(draw_text_buffer);

    switch(ALIGN)
    {
        case TEXT_RIGHT:
            X -= (len * GLYPH_WIDTH);
            break;

        case TEXT_CENTER:
            X -= (len * GLYPH_WIDTH) / 2;
            break;
    }

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(font_texture, R, G, B);

    for(i = 0; i < len; i++)
    {
        c = draw_text_buffer[i];

        if(c >= ' ' AND c <= 'Z')
        {
            rect.x = (c - ' ') * GLYPH_WIDTH;

            blit_rect(font_texture, &rect, X, Y, 1.f);

            X += GLYPH_WIDTH;
        }
    }
};
