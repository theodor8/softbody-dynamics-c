#include "renderer.h"

#include "SDL2/SDL.h"

#include "vec2.h"


#define K_SEGMENTS 64.0f
static const float k_increment = 2.0f * M_PI / K_SEGMENTS;



Renderer *renderer_create(int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);
    Renderer *r = calloc(1, sizeof(Renderer));
    SDL_CreateWindowAndRenderer(width, height, 0, &r->window, &r->renderer);
    SDL_SetRenderDrawBlendMode(r->renderer, SDL_BLENDMODE_BLEND);
    return r;
}

void renderer_destroy(Renderer *r)
{
    SDL_DestroyRenderer(r->renderer);
    SDL_DestroyWindow(r->window);
    free(r);
    SDL_Quit();
}

void renderer_draw_line(Renderer *r, Vec2 v1, Vec2 v2)
{
    SDL_RenderDrawLineF(r->renderer, v1.x, v1.y, v2.x, v2.y);
}

void renderer_draw_filled_solid_triangle(Renderer *r, Vec2 v1, Vec2 v2, Vec2 v3)
{
    SDL_Color color;
    SDL_GetRenderDrawColor(r->renderer, &color.r, &color.g, &color.b, &color.a);
    renderer_draw_filled_triangle(r, v1, v2, v3, color, color, color);
}

void renderer_draw_filled_triangle(Renderer *r, Vec2 v1, Vec2 v2, Vec2 v3, SDL_Color c1, SDL_Color c2, SDL_Color c3)
{
    SDL_Vertex vs[] = {
        {{v1.x, v1.y}, c1},
        {{v2.x, v2.y}, c2},
        {{v3.x, v3.y}, c3},
    };
    SDL_RenderGeometry(r->renderer, NULL, vs, 3, NULL, 0);
}

void renderer_draw_circle(Renderer *r, Vec2 center, float radius)
{
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 r1 = vec2(1.0f, 0.0f);
    Vec2 v1 = vec2_add(center, vec2_mulf(r1, radius));
    for (int i = 0; i < K_SEGMENTS; ++i)
    {
        Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        Vec2 v2 = vec2_add(center, vec2_mulf(r2, radius));
        renderer_draw_line(r, v1, v2);
        r1 = r2;
        v1 = v2;
    }
}

void renderer_draw_filled_circle(Renderer *r, Vec2 center, float radius)
{
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 v0 = center;
    Vec2 r1 = vec2(cosInc, sinInc);
    Vec2 v1 = vec2_add(center, vec2_mulf(r1, radius));
    for (int i = 0; i < K_SEGMENTS; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        Vec2 v2 = vec2_add(center, vec2_mulf(r2, radius));
        renderer_draw_filled_solid_triangle(r, v0, v1, v2);
        r1 = r2;
        v1 = v2;
    }
}
