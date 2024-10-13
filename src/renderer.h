#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "vec2.h"

typedef struct Renderer Renderer;

Renderer *renderer_create(int width, int height);
void renderer_destroy(Renderer *renderer);
void renderer_draw_line(Renderer *renderer, Vec2 v1, Vec2 v2);
void renderer_draw_filled_solid_triangle(Renderer *renderer, Vec2 v1, Vec2 v2, Vec2 v3);
void renderer_draw_filled_triangle(Renderer *renderer, Vec2 v1, Vec2 v2, Vec2 v3, SDL_Color c1, SDL_Color c2, SDL_Color c3);
void renderer_draw_circle(Renderer *renderer, Vec2 center, float radius);
void renderer_draw_filled_circle(Renderer *renderer, Vec2 center, float radius);
void renderer_set_draw_color(Renderer *renderer, int r, int g, int b, int a);
void renderer_present(Renderer *renderer);
void renderer_clear(Renderer *renderer);
