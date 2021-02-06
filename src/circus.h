#ifndef CIRCUS_H
#define CIRCUS_H

#include "game.h"
#define C_IMG "../img/circus.png"
#define C_HEIGHT 64
#define C_WIDTH 64

struct circus {
        SDL_Rect r;
};

void circus_render(SDL_Renderer *r, 
                   SDL_Texture *t,
                   struct circus *c);
#endif
