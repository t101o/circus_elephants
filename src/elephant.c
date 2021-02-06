#include "elephant.h"

void elephant_render(SDL_Renderer *r,
                     SDL_Texture *alive,
                     SDL_Texture *dead,
                     struct elephant *e,
                     const int mx,
                     const int my)
{
        /* Vector from elephant to mouse pointer */
        struct vector v;
        
        /* Calculating vector coordinates */
        v.x = mx - ((e->r).x + (e->r).w/2);
        v.y = my - ((e->r).y + (e->r).h/2);
        
        /* Length of the vector (distance) */
        float d;
        d = sqrt(v.x * v.x + v.y * v.y);
        /*v.x /= d;
        v.y /= d;
        */
       // printf("%f %f %f\n", v.x, v.y, d);
        
        /* If the mouse pointer is not too close, elephant stays alive and moves */
        if (e->alive && d <= E_M_DISTANCE_MAX && d >= E_M_DISTANCE_MIN) {
                /* Changing velocity sign determined by the vector */
                (e->r).x -= e->velocity * ((v.x >= 0) - (v.x <= 0));
                (e->r).y -= e->velocity * ((v.y >= 0) - (v.y <= 0));
                
                /* Out of bounds checking, with 5px frame */
                if ((e->r).x <= F_MARGIN) {
                        (e->r).x = F_MARGIN;
                }
                if ((e->r).x >= W_WIDTH - (e->r).w - F_MARGIN) {
                        (e->r).x = W_WIDTH - (e->r).w - F_MARGIN;
                }
                if ((e->r).y <= F_MARGIN) {
                        (e->r).y = F_MARGIN;
                }
                if ((e->r).y + F_MARGIN >= W_HEIGHT - (e->r).h) {
                        (e->r).y = W_HEIGHT - (e->r).h - F_MARGIN;
                        /*printf(">y%d\n", (e->r).y);*/
                }
        /* If the mouse pointer is too close, elephant dies from fear */        
        } else if (e->alive && d < E_M_DISTANCE_MIN) {
                e->alive = 0;
        }
        
        /* Copy the appropriate elephant texture to Copy buffer */
        if (e->alive) {
                SDL_RenderCopy(r, alive, NULL, &(e->r));
        } else {
                SDL_RenderCopy(r, dead, NULL, &(e->r));
        }
}

void elephant_init(struct elephant *el, const int mx, const int my)
{
        int qm = quadrant_get(mx, my);
        do {
                do {
                        el->r.x = rand() % (W_WIDTH - (E_WIDTH + F_MARGIN)) + F_MARGIN;
                } while ((el->r.x - E_WIDTH/2) > (W_WIDTH/2) - E_C_DISTANCE_SPAWN && 
                        (el->r.x - E_WIDTH/2) < (W_WIDTH/2) + E_C_DISTANCE_SPAWN);
                
                do {
                        el->r.y = rand() % (W_HEIGHT - (E_HEIGHT + F_MARGIN)) + F_MARGIN;
                } while ((el->r.y - E_HEIGHT/2) > (W_HEIGHT/2) - E_C_DISTANCE_SPAWN &&
                        (el->r.y - E_HEIGHT/2) < (W_HEIGHT/2) + E_C_DISTANCE_SPAWN);
        } while (qm == quadrant_get(el->r.x, el->r.y));
        
        el->r.h = E_HEIGHT;
        el->r.w = E_WIDTH;
        el->alive = 1;
        el->velocity = rand() % (E_VELOCITY_MAX - 1) + E_VELOCITY_MIN;
        el->frame_count = 0;
}
