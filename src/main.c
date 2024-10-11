#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "common.h"
#include "renderer.h"
#include "softbody.h"
#include "map.h"





int main(void)
{
    srand(time(NULL));

    Renderer *r = renderer_create(WIDTH, HEIGHT);

    //Softbody *sb = softbody_create_rect(vec2(100, 100), vec2(50, 100));
    Softbody *sb = softbody_create_circle(vec2(200, 100), 50, 10);

    Map *map = map_create();
    map_add_edge(map, vec2(20, 20), vec2(WIDTH - 20, 20));
    map_add_edge(map, vec2(WIDTH - 20, 20), vec2(WIDTH - 20, HEIGHT - 20));
    map_add_edge(map, vec2(WIDTH - 20, HEIGHT - 20), vec2(20, HEIGHT - 20));
    map_add_edge(map, vec2(20, HEIGHT - 20), vec2(20, 20));


    // Uint32 last_ticks = SDL_GetTicks();
    Vec2 mouse_hold_start;
    bool mouse_holding = false;
    bool running = false;
    bool quit = false;
    while (!quit)
    {
        int mx, my;
        Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
        Vec2 mouse_pos = vec2(mx, my);

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

                        case SDLK_SPACE:
                            running = !running;
                            break;

                        case SDLK_s:
                            if (!running)
                            {
                                softbody_update(sb, map);
                            }

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

        SDL_SetRenderDrawColor(r->renderer, 255, 150, 150, 255);
        if (SDL_BUTTON(mouse_buttons) == SDL_BUTTON_LEFT)
        {
            if (mouse_holding)
            {
                renderer_draw_line(r, mouse_hold_start, mouse_pos);
            }
            else
            {
                mouse_holding = true;
                mouse_hold_start = mouse_pos;
            }
        }
        else if (mouse_holding)
        {
            mouse_holding = false;
            map_add_edge(map, mouse_hold_start, mouse_pos);
        }

        // if (running && SDL_BUTTON(mouse_buttons) == SDL_BUTTON_LEFT)
        // {
        //     Vec2 c = softbody_center(sb);
        //     Vec2 f = vec2_mulf(vec2_normalized(vec2_sub(mouse_pos, c)), 5);
        //     softbody_apply_force_closest(sb, mouse_pos, f);
        // }

        // Uint32 curr_ticks = SDL_GetTicks();
        // double dt = (float)(curr_ticks - last_ticks) / 1000.0f;
        // last_ticks = curr_ticks;
        if (running)
        {
            softbody_update(sb, map);
        }

        softbody_render(sb, r);
        map_render(map, r);



        SDL_RenderPresent(r->renderer);
        SDL_Delay(30);

    }
    

    softbody_destroy(sb);
    map_destroy(map);

    renderer_destroy(r);

    return 0;
}


