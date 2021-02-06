#include "game.h"

int quadrant_get(const int x, const int y)
{
        return (x < W_WIDTH/2) * 1 + (y < W_HEIGHT/2) * 2;
}
 
