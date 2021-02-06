#ifndef ELEPHANT_H
#define ELEPHANT_H

#include "game.h"
#include <math.h>

#define E_ALIVE_IMG "../img/elephant_alive.png"
#define E_DEAD_IMG "../img/elephant_dead.png"
#define E_VELOCITY_MIN 1
#define E_VELOCITY_MAX 4
#define E_HEIGHT 32
#define E_WIDTH 32
#define E_COUNT_MAX 15
#define E_M_DISTANCE_MAX 48.0
#define E_M_DISTANCE_MIN 14.0
#define E_C_DISTANCE_MIN 24.0
#define E_C_DISTANCE_SPAWN 64.0 

struct elephant {
        SDL_Rect r;
        int alive;
        int velocity;
        int frame_count;
};

void elephant_render(SDL_Renderer *r,
                     SDL_Texture *alive,
                     SDL_Texture *dead,
                     struct elephant *e,
                     const int mx,
                     const int my);

void elephant_init(struct elephant *el, const int mx, const int my);
#endif
