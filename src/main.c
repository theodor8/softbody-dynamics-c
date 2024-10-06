#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "common.h"
#include "renderer.h"
#include "softbody.h"





int main(void)
{
    srand(time(NULL));

    Renderer *r = renderer_create(WIDTH, HEIGHT);

    //Softbody *sb = softbody_create_rect(vec2(200, 100), vec2(100, 200));
    Softbody *sb = softbody_create_circle(vec2(200, 100), 50, 7);




    bool quit = false;
    while (!quit)
    {
        int mx, my;
        Uint32 mb = SDL_GetMouseState(&mx, &my);
        SDL_GetMouseState(&mx, &my);
        Vec2 mp = vec2(mx, my);

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

        if (SDL_BUTTON(mb) == SDL_BUTTON_LEFT)
        {
            Vec2 c = softbody_center(sb);
            Vec2 f = vec2_mulf(vec2_normalized(vec2_sub(mp, c)), 5);
            softbody_apply_force_closest(sb, mp, f);
        }

        SDL_SetRenderDrawColor(r->renderer, 30, 30, 30, 255);
        SDL_RenderClear(r->renderer);
        SDL_SetRenderDrawColor(r->renderer, 255, 255, 255, 255);

        softbody_update(sb);

        softbody_render(sb, r);

        SDL_RenderPresent(r->renderer);
        SDL_Delay(30);

    }
    

    softbody_destroy(sb);

    renderer_destroy(r);

    return 0;
}


