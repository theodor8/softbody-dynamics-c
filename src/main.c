#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "renderer.h"
#include "world.h"


#define WIDTH 600
#define HEIGHT 600


void connect_points(World *world, Point *pts[], int pts_siz, float spring_strength, float spring_damping)
{
    for (int i = 0; i < pts_siz; ++i)
    {
        for (int j = i + 1; j < pts_siz; ++j)
        {
            world_add_spring(world, pts[i], pts[j], spring_strength, spring_damping);
        }
    }
}


void create_rect(World *world, Vec2 pos, Vec2 siz, float spring_strength, float spring_damping)
{
    Point *pts[4];
    pts[0] = world_add_pt(world, pos);
    pts[1] = world_add_pt(world, vec2(pos.x + siz.x, pos.y));
    pts[2] = world_add_pt(world, vec2(pos.x + siz.x, pos.y + siz.y));
    pts[3] = world_add_pt(world, vec2(pos.x, pos.y + siz.y));
    connect_points(world, pts, 4, spring_strength, spring_damping);

}

void create_filled_rect(World *world, Vec2 pos, Vec2 siz, int rows, int cols, float spring_strength, float spring_damping)
{
    Point *pts[rows * cols];
    siz.x /= cols;
    siz.y /= rows;
    for (int r = 0; r < rows; ++r)
    {
        float y = pos.y + siz.y * r;
        for (int c = 0; c < cols; ++c)
        {
            float x = pos.x + siz.x * c;
            pts[r * cols + c] = world_add_pt(world, vec2(x, y));
        }
    }

    // Connect points
    for (int i = 0; i < rows * cols; ++i)
    {
        Point *pt = pts[i];
        if (i % cols != 0) // Left
        {
            world_add_spring(world, pt, pts[i - 1], spring_strength, spring_damping);
        }
        if (i >= cols) // Up
        {
            world_add_spring(world, pt, pts[i - cols], spring_strength, spring_damping);
        }
        if (i % cols != 0 && i >= cols) // Left Up
        {
            world_add_spring(world, pt, pts[i - cols - 1], spring_strength, spring_damping);
        }
        if (i % cols != 0 && i < cols * (rows - 1)) // Left Down
        {
            world_add_spring(world, pt, pts[i + cols - 1], spring_strength, spring_damping);
        }
    }
}

void create_circle(World *world, Vec2 pos, float r, int num_pts, float spring_strength, float spring_damping)
{
    Point *pts[num_pts];
    float da = 2 * M_PI / num_pts;
    for (int i = 0; i < num_pts; ++i)
    {
        float a = da * i;
        Vec2 pt = vec2_add(pos, vec2(cosf(a) * r, sinf(a) * r));
        pts[i] = world_add_pt(world, pt);
    }
    connect_points(world, pts, num_pts, spring_strength, spring_damping);
}



int main(void)
{
    srand(time(NULL));

    Renderer *renderer = renderer_create(WIDTH, HEIGHT);
    // const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    World *world = world_create(0.35, 0.01, 0.2);

    // create_rect(world, vec2(90, 100), vec2(50, 100), 0.08, 0.05);
    // create_rect(world, vec2(100, 350), vec2(200, 100), 0.08, 0.05);
    // create_circle(world, vec2(200, 100), 50, 10, 0.06f, 0.1f);
    create_filled_rect(world, vec2(200, 100), vec2(150, 225), 5, 4, 0.25f, 0.09f);

    world_add_edge(world, vec2(0, 20), vec2(WIDTH, 20));
    world_add_edge(world, vec2(WIDTH - 20, 0), vec2(WIDTH - 20, HEIGHT));
    world_add_edge(world, vec2(WIDTH, HEIGHT - 20), vec2(0, HEIGHT - 20));
    world_add_edge(world, vec2(20, HEIGHT), vec2(20, 0));



    
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
                                world_step(world);
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

        renderer_set_draw_color(renderer, 30, 30, 30, 255);
        renderer_clear(renderer);


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
                    renderer_set_draw_color(renderer, 255, 150, 150, 255);
                    renderer_draw_line(renderer, mouse_hold_start, mouse_pos);
                }
            }
            else
            {
                mouse_holding = true;
                mouse_hold_start = mouse_pos;

                // for (int i = 0; i < map->edges_i; ++i)
                // {
                //     if (vec2_dist(mouse_pos, map->edges[i].p1) < 15)
                //     {
                //         mouse_dragging_edge = &map->edges[i].p1;
                //         break;
                //     }
                //     if (vec2_dist(mouse_pos, map->edges[i].p2) < 15)
                //     {
                //         mouse_dragging_edge = &map->edges[i].p2;
                //         break;
                //     }
                // }
            }
        }
        else if (mouse_holding)
        {
            mouse_holding = false;
            if (!mouse_dragging_edge && !vec2_equal(mouse_hold_start, mouse_pos))
            {
                world_add_edge(world, mouse_hold_start, mouse_pos);
            }
            mouse_dragging_edge = NULL;
        }


        // Apply force
        // if (running && keyboard_state[SDL_SCANCODE_F])
        // {
        //     for (int i = 0; i < sbs_i; ++i)
        //     {
        //         Vec2 c = sb_center(sbs[i]);
        //         Vec2 f = vec2_mulf(vec2_normalized(vec2_sub(mouse_pos, c)), 3);
        //         sb_apply_force_closest(sbs[i], mouse_pos, f);
        //     }
        // }

        Uint64 curr_ticks = SDL_GetTicks64();
        if ((running))
        {
            delta_ticks += curr_ticks - prev_ticks;
            while (delta_ticks >= 30)
            {
                world_step(world);
                delta_ticks -= 30;
            }
        }
        prev_ticks = curr_ticks;


        world_render(world, renderer);


        renderer_present(renderer);
        SDL_Delay(10);

    }
    

    world_destroy(world);

    renderer_destroy(renderer);

    return 0;
}


