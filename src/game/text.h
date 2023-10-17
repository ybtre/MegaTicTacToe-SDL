#ifndef TEXT_H
#define TEXT_H

#define GLYPH_HEIGHT    28
#define GLYPH_WIDTH     18

void init_fonts(void);
void draw_text(int X, int Y, int R, int G, int B, int ALIGN, const char *FORMAT, ...);

#endif
