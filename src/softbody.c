#include "softbody.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

#include "vec2.h"
#include "renderer.h"

typedef struct
{
    int i1, i2; // indices to pts array
    float len; // original length
} Spring;


struct Softbody
{
    Vec2 *pts;
    size_t pts_siz;
    int pts_i;

    Spring *springs;
    size_t springs_siz;
    int springs_i;
};


Softbody *softbody_create(void)
{
    Softbody *sb = calloc(1, sizeof(Softbody));

    sb->pts_siz = 10;
    sb->pts = calloc(sb->pts_siz, sizeof(Vec2));

    sb->springs_siz = 10;
    sb->springs = calloc(sb->springs_siz, sizeof(Spring));

    return sb;
}

void softbody_destroy(Softbody *sb)
{
    free(sb->pts);
    free(sb->springs);
    free(sb);
}

int softbody_add_pt(Softbody *sb, Vec2 pt)
{
    if (sb->pts_i >= sb->pts_siz)
    {
        sb->pts_siz *= 2;
        sb->pts = realloc(sb->pts, sb->pts_siz);
    }

    sb->pts[sb->pts_i] = pt;
    ++sb->pts_i;

    return sb->pts_i - 1;
}

int softbody_add_spring(Softbody *sb, int i1, int i2)
{
    if (sb->springs_i >= sb->springs_siz)
    {
        sb->springs_siz *= 2;
        sb->springs = realloc(sb->springs, sb->springs_siz);
    }

    sb->springs[sb->springs_i].i1 = i1;
    sb->springs[sb->springs_i].i2 = i2;
    sb->springs[sb->springs_i].len = vec2_dist(sb->pts[i1], sb->pts[i2]);
    ++sb->springs_i;

    return sb->springs_i - 1;
}

void softbody_connect_pts(Softbody *sb)
{
    for (int i1 = 0; i1 < sb->pts_i; ++i1)
    {
        for (int i2 = i1 + 1; i2 < sb->pts_i; ++i2)
        {
            softbody_add_spring(sb, i1, i2);
        }
    }
}

void softbody_render(Softbody *sb, Renderer *r)
{
    SDL_SetRenderDrawColor(r->renderer, 0, 255, 255, 255);
    for (int i = 0; i < sb->pts_i; ++i)
    {
        renderer_draw_filled_circle(r, sb->pts[i], 5);
    }

    SDL_SetRenderDrawColor(r->renderer, 255, 0, 0, 255);
    for (int i = 0; i < sb->springs_i; ++i)
    {
        Vec2 p1 = sb->pts[sb->springs[i].i1];
        Vec2 p2 = sb->pts[sb->springs[i].i2];
        renderer_draw_line(r, p1, p2);
    }
}

