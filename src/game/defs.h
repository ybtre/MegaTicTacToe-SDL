#ifndef DEFS_H
#define DEFS_H

//draw debug rects
//#define DRAW_RECTS

#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           160
#define SCREEN_SCALE            4                   // x4 equals 1280x720

#define FPS                     60

#define PI                      3.14159265358979323846

#define PLAYER_SPEED            20
#define PLAYER_DELTA            .5

#define PLAYER_GREEN            1
#define PLAYER_ORANGE           2

#define GRID_SIZE               9
#define GRID_X                  3
#define GRID_Y                  3

#define ENTITIES_MAX            64


#define MAX_NAME_LENGTH         32
#define MAX_LINE_LENGTH         1024

#define MAX_KEYBOARD_KEYS       350
#define MAX_MOUSE_BUTTONS       6

#define MAX_SND_CHANNELS        16

enum
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};

#define inline static


#undef MIN 
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#undef MAX 
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#undef AVG
#define AVG(a, b) (((a) + (b)) / 2.)
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'


#define NOT     !
#define NOTEQ   !=
#define AND     &&
#define OR      ||
#define MOD     %
#define elif    else if
#define true    1
#define false   0

#define ifnull( _ ) if( ( _ ) == NULL)

#endif
