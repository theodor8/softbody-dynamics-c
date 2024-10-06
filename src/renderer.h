#pragma once

#include "SDL2/SDL.h"

#include "vec2.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Renderer;

Renderer *renderer_create(int width, int height);

void renderer_destroy(Renderer *r);

void renderer_draw_line(Renderer *r, Vec2 v1, Vec2 v2);

void renderer_draw_filled_solid_triangle(Renderer *r, Vec2 v1, Vec2 v2, Vec2 v3);

void renderer_draw_filled_triangle(Renderer *r, Vec2 v1, Vec2 v2, Vec2 v3, SDL_Color c1, SDL_Color c2, SDL_Color c3);

void renderer_draw_circle(Renderer *r, Vec2 center, float radius);

void renderer_draw_filled_circle(Renderer *r, Vec2 center, float radius);

