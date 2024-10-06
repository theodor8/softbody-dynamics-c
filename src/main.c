#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "renderer.h"
#include "softbody.h"

#define WIDTH 600
#define HEIGHT 600



int main(void)
{
    srand(time(NULL));

    Renderer *r = renderer_create(WIDTH, HEIGHT);

    Softbody *sb = softbody_create();

    softbody_add_pt(sb, vec2(100, 100));
    softbody_add_pt(sb, vec2(100, 200));
    softbody_add_pt(sb, vec2(200, 100));
    softbody_add_pt(sb, vec2(200, 200));
    softbody_connect_pts(sb);


    bool quit = false;
    while (!quit)
    {
        int mx, my;
        //Uint32 mb = SDL_GetMouseState(&mx, &my);
        SDL_GetMouseState(&mx, &my);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }



        SDL_SetRenderDrawColor(r->renderer, 30, 30, 30, 255);
        SDL_RenderClear(r->renderer);
        SDL_SetRenderDrawColor(r->renderer, 255, 255, 255, 255);


        softbody_render(sb, r);

        SDL_RenderPresent(r->renderer);
        SDL_Delay(30);

    }
    

    softbody_destroy(sb);

    renderer_destroy(r);

    return 0;
}


