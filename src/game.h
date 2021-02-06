#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"

#define FONT "../fonts/Pixellettersfull.ttf"
#define FONT_SIZE 32

#define B_RED 244
#define B_GREEN 181
#define B_BLUE 83

#define W_HEIGHT 480
#define W_WIDTH 640

#define F_RATE 30
#define F_MARGIN 5

#define S_DEATH -100
#define S_SAVED 50

#define G_TIME 120

struct vector {
        float x;
        float y;
};
 
int quadrant_get(const int x, const int y);
#endif
