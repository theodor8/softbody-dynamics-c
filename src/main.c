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



        SDL_SetRenderDrawColor(r->renderer, 200, 200, 200, 255);
        SDL_RenderClear(r->renderer);
        SDL_SetRenderDrawColor(r->renderer, 0, 0, 0, 255);

        
        

        SDL_RenderPresent(r->renderer);
        SDL_Delay(30);

    }
    


    renderer_destroy(r);

    return 0;
}


