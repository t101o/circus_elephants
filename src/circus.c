#include "circus.h"

void circus_render(SDL_Renderer *r, 
                   SDL_Texture *t,
                   struct circus *c)
{
        SDL_RenderCopy(r, t, NULL, &(c->r));
} 
