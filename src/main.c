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


    //sbs[sbs_i++] = sb_create_rect(vec2(100, 100), vec2(50, 100), 0.3);
    sbs[sbs_i++] = sb_create_circle(vec2(200, 100), 50, 10, 0.06f);
    //sbs[sbs_i++] = sb_create_filled_rect(vec2(200, 100), vec2(100, 200), 6, 5, 0.45f);

    Map *map = map_create();
    map_add_edge(map, vec2(20, 20), vec2(WIDTH - 20, 20));
    map_add_edge(map, vec2(WIDTH - 20, 20), vec2(WIDTH - 20, HEIGHT - 20));
    map_add_edge(map, vec2(WIDTH - 20, HEIGHT - 20), vec2(20, HEIGHT - 20));
    map_add_edge(map, vec2(20, HEIGHT - 20), vec2(20, 20));



    
    Vec2 mouse_hold_start;
    bool mouse_holding = false;
    Vec2 *mouse_dragging_edge = NULL;

    bool step_update = false;
    bool running = false;
    bool quit = false;

    // Uint64 prev_ticks = SDL_GetTicks64();
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
                                step_update = true;
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
                    if (vec2_dist(mouse_pos, map->edges[i].p1) < 20)
                    {
                        mouse_dragging_edge = &map->edges[i].p1;
                        break;
                    }
                    if (vec2_dist(mouse_pos, map->edges[i].p2) < 20)
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
                Vec2 f = vec2_mulf(vec2_normalized(vec2_sub(mouse_pos, c)), 5);
                sb_apply_force_closest(sbs[i], mouse_pos, f);
            }
        }

        // Uint64 curr_ticks = SDL_GetTicks64();
        // float dt = (curr_ticks - prev_ticks) / 1000.0f;
        // prev_ticks = curr_ticks;
        if (running || step_update)
        {
            step_update = false;
            for (int i = 0; i < sbs_i; ++i)
            {
                sb_update(sbs[i], map);
            }
        }

        for (int i = 0; i < sbs_i; ++i)
        {
            sb_render(sbs[i], r);
        }

        map_render(map, r);

        SDL_RenderPresent(r->renderer);
        SDL_Delay(30);

    }
    

    for (int i = 0; i < sbs_i; ++i)
    {
        sb_destroy(sbs[i]);
    }
    map_destroy(map);

    renderer_destroy(r);

    return 0;
}


