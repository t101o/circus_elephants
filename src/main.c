#include "game.h"
#include "elephant.h"
#include "circus.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>

/**
 * TODO Obstacles
 **/

int elephant_in_circus(const struct elephant *e, const struct circus *c)
{
        /* Vector between elephant and circus */
        struct vector v;
        v.x = ((c->r).x + (c->r).w/2) - ((e->r).x + (e->r).w/2);
        v.y = ((c->r).y + (c->r).h/2) - ((e->r).y + (e->r).h/2);
        
        float d = sqrt(v.x * v.x + v.y * v.y);
        
        return (e->alive && d <= E_C_DISTANCE_MIN);
}

int main (void)
{
        /* RNG seed */
        srand(time(NULL));
        
        SDL_Window      *window         = NULL;        /* Window structure */
        SDL_Renderer    *renderer       = NULL;        /* Renderer structure */
        SDL_Surface     *surface        = NULL;        /* Surface for making textures */
        SDL_Texture     *t_el_alive     = NULL;        /* Elephant alive texture */
        SDL_Texture     *t_el_dead      = NULL;        /* Elephant dead texture */
        SDL_Texture     *t_circus       = NULL;        /* Circus texture */
        TTF_Font        *font           = NULL;        /* Font */
        SDL_Texture     *t_score        = NULL;        /* Score texture */
        SDL_Texture     *t_time         = NULL;        /* Time texture */
        int error = 0;
        
        /* Initialize SDL video */
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("Failed to initialize SDL: %s\n", SDL_GetError());
                error = 1;
                goto Cleanup;
        }
       
        /*printf(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_INFO,
                       "SDL initialized correctly!\n");
        */
        /* Creating window */
        window = SDL_CreateWindow("Window",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  W_WIDTH,
                                  W_HEIGHT,
                                  SDL_WINDOW_OPENGL/* | SDL_WINDOW_FULLSCREEN*/);
        if (!window) {
                printf("Failed to create window: %s\n", SDL_GetError());
                error = 1;
                goto Cleanup;
        }
        
        /* Create renderer */
        renderer = SDL_CreateRenderer(window,
                                      -1,
                                      SDL_RENDERER_ACCELERATED |
                                      SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
                printf("Failed to create renderer: %s\n", SDL_GetError());
                error = 1;
                goto Cleanup;
        }
        
        if (TTF_Init() < 0) {
                printf("Failed to initialize TTF: %s\n", TTF_GetError());
                error = 1;
                goto Cleanup;
        }
        
        /* Making textures from game images */
        if (IMG_Init(IMG_INIT_PNG) < 0) {
                printf("Failed to initialize IMG: %s\n", IMG_GetError());
                error = 1;
                goto Cleanup;
        }
        
        surface = IMG_Load(E_ALIVE_IMG);
        if (!surface) {
                printf("Failed to open image: %s\n", IMG_GetError());
                error = 1;
                goto Cleanup;
        }
        t_el_alive = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        surface = IMG_Load(E_DEAD_IMG);
        if (!surface) {
                printf("Failed to open image: %s\n", IMG_GetError());
                error = 1;
                goto Cleanup;
        }
        t_el_dead = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        surface = IMG_Load(C_IMG);
        if (!surface) {
                printf("Failed to open image: %s\n", IMG_GetError());
                error = 1;
                goto Cleanup;
        }
        t_circus = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        
        font = TTF_OpenFont(FONT, FONT_SIZE);
        if (!font) {
                /* TODO Font opening error */
                printf("Failed to open font file: %s\n", TTF_GetError());
                error = 1;
                goto Cleanup;
        }
        
        
        
        /* Background color */
        SDL_SetRenderDrawColor(renderer, B_RED, B_GREEN, B_BLUE, 0);
        /* Font color */
        SDL_Color font_color = {0,0,0,0};
        
        
        struct elephant el[E_COUNT_MAX];
        for (int i = 0; i < E_COUNT_MAX; i++) {
                elephant_init(&el[i], rand() % W_WIDTH, rand() % W_HEIGHT);
        }
        
        struct circus c;
        c.r.x = (W_WIDTH - C_WIDTH)/2;
        c.r.y = (W_HEIGHT - C_WIDTH)/2;
        c.r.h = C_HEIGHT;
        c.r.w = C_WIDTH;
        
        /* Rendering/Game loop */
        int score = 0;
        int quit = 0;
        int mx, my; /* Mouse coordinates */
        int f_count = 0;
        SDL_Event e;
        char score_text[6];
        char time_text[4];
        
        SDL_Rect score_box;
        score_box.y = F_MARGIN;
        
        SDL_Rect time_box;
        time_box.y = F_MARGIN;
        time_box.x = F_MARGIN;
        
        while (!quit) {
                while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) {
                                quit = 1;
                        }
                }
                
                SDL_RenderClear(renderer);
                
                if (f_count == G_TIME * F_RATE) {
                        /*printf("\n\n%d\n\n", score);*/
                        /* TODO Display leaderboard or score */
                        circus_render(renderer, t_circus, &c);
                } else {
                        SDL_GetMouseState(&mx, &my);
                        circus_render(renderer, t_circus, &c);
                        /* Render elephants */
                        for (int i = 0; i < E_COUNT_MAX; i++) {
                                if (!elephant_in_circus(&el[i], &c)) {
                                        elephant_render(renderer, 
                                                        t_el_alive, 
                                                        t_el_dead, 
                                                        &el[i], 
                                                        mx, 
                                                        my);
                                } else {
                                        if (el[i].frame_count == 0) {
                                                score += S_SAVED;
                                        }
                                        el[i].frame_count++;
                                }
                                
                                if (!el[i].alive) {
                                        if (el[i].frame_count == 0) {
                                                score += S_DEATH;
                                        }
                                        el[i].frame_count++;
                                }
                                //printf("%d\n\n", score);
                                if (el[i].frame_count == F_RATE/2) {
                                        elephant_init(&el[i], mx, my);
                                }
                        }
                        f_count++;
                }
                
                sprintf(score_text, "%5d", score);
                surface = TTF_RenderText_Solid(font, score_text, font_color);
                score_box.h = surface->h;
                score_box.w = surface->w;
                score_box.x = W_WIDTH - score_box.w - F_MARGIN;
                t_score = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                SDL_RenderCopy(renderer, t_score, NULL, &score_box);
                
                
                sprintf(time_text, "%3d", G_TIME - f_count / F_RATE);
                surface = TTF_RenderText_Solid(font, time_text, font_color);
                time_box.h = surface->h;
                time_box.w = surface->w;
                t_time = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                SDL_RenderCopy(renderer, t_time, NULL, &time_box);
                
                SDL_DestroyTexture(t_score);
                SDL_DestroyTexture(t_time);
                
                SDL_RenderPresent(renderer);
                SDL_Delay(1000/F_RATE);
        }
        
Cleanup:
        /* Cleanup */
        SDL_DestroyTexture(t_el_alive);
        SDL_DestroyTexture(t_el_dead);
        SDL_DestroyTexture(t_circus);
        SDL_DestroyTexture(t_score);
        SDL_DestroyTexture(t_time);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        
        return error;
}
