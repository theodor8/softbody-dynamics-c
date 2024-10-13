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
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    Softbody *sbs[10] = {0};
    int sbs_i = 0; 

    //sbs[sbs_i++] = sb_create_rect(vec2(100, 100), vec2(50, 100), 0.08, 0.05);
    //sbs[sbs_i++] = sb_create_circle(vec2(200, 100), 50, 10, 0.06f, 0.1f);
    sbs[sbs_i++] = sb_create_filled_rect(vec2(200, 100), vec2(150, 225), 5, 4, 0.25f, 0.09f);

    Map *map = map_create();
    map_add_edge(map, vec2(0, 20), vec2(WIDTH, 20));
    map_add_edge(map, vec2(WIDTH - 20, 0), vec2(WIDTH - 20, HEIGHT));
    map_add_edge(map, vec2(WIDTH, HEIGHT - 20), vec2(0, HEIGHT - 20));
    map_add_edge(map, vec2(20, HEIGHT), vec2(20, 0));



    
    Vec2 mouse_hold_start;
    bool mouse_holding = false;
    Vec2 *mouse_dragging_edge = NULL;

    bool running = false;
    bool quit = false;

    Uint64 prev_ticks = SDL_GetTicks64();
    Uint64 delta_ticks = 0;
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
                                for (int i = 0; i < sbs_i; ++i)
                                {
                                    sb_update(sbs[i], map);
                                }
                            }
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


        // Mouse controls
        if (SDL_BUTTON(mouse_buttons) == SDL_BUTTON_LEFT)
        {
            if (mouse_holding)
            {
                if (mouse_dragging_edge)
                {
                    *mouse_dragging_edge = mouse_pos;
                }
                else
                {
                    SDL_SetRenderDrawColor(r->renderer, 255, 150, 150, 255);
                    renderer_draw_line(r, mouse_hold_start, mouse_pos);
                }
            }
            else
            {
                mouse_holding = true;
                mouse_hold_start = mouse_pos;

                for (int i = 0; i < map->edges_i; ++i)
                {
                    if (vec2_dist(mouse_pos, map->edges[i].p1) < 15)
                    {
                        mouse_dragging_edge = &map->edges[i].p1;
                        break;
                    }
                    if (vec2_dist(mouse_pos, map->edges[i].p2) < 15)
                    {
                        mouse_dragging_edge = &map->edges[i].p2;
                        break;
                    }
                }
            }
        }
        else if (mouse_holding)
        {
            mouse_holding = false;
            if (!mouse_dragging_edge && !vec2_equal(mouse_hold_start, mouse_pos))
            {
                map_add_edge(map, mouse_hold_start, mouse_pos);
            }
            mouse_dragging_edge = NULL;
        }


        // Apply force
        if (running && keyboard_state[SDL_SCANCODE_F])
        {
            for (int i = 0; i < sbs_i; ++i)
            {
                Vec2 c = sb_center(sbs[i]);
                Vec2 f = vec2_mulf(vec2_normalized(vec2_sub(mouse_pos, c)), 3);
                sb_apply_force_closest(sbs[i], mouse_pos, f);
            }
        }

        Uint64 curr_ticks = SDL_GetTicks64();
        if ((running))
        {
            delta_ticks += curr_ticks - prev_ticks;
            while (delta_ticks >= 30)
            {
                for (int i = 0; i < sbs_i; ++i)
                {
                    sb_update(sbs[i], map);
                }
                delta_ticks -= 30;
            }
        }
        prev_ticks = curr_ticks;


        for (int i = 0; i < sbs_i; ++i)
        {
            sb_render(sbs[i], r);
        }

        map_render(map, r);

        SDL_RenderPresent(r->renderer);
        SDL_Delay(10);

    }
    

    for (int i = 0; i < sbs_i; ++i)
    {
        sb_destroy(sbs[i]);
    }
    map_destroy(map);

    renderer_destroy(r);

    return 0;
}


