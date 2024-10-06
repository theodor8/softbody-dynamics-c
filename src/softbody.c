#include "softbody.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

#include "common.h"
#include "vec2.h"
#include "renderer.h"
#include "map.h"

typedef struct
{
    Vec2 pos, vel;
} Point;

typedef struct
{
    int i1, i2; // indices to pts array
    float len; // original length
} Spring;

struct Softbody
{
    Point *pts; // cw
    int pts_siz;
    int pts_i;

    Spring *springs;
    int springs_siz;
    int springs_i;
};


Softbody *softbody_create(void)
{
    Softbody *sb = calloc(1, sizeof(Softbody));

    sb->pts_siz = 10;
    sb->pts = calloc(sb->pts_siz, sizeof(Point));

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
        sb->pts = realloc(sb->pts, sizeof(Point) * sb->pts_siz);
    }

    sb->pts[sb->pts_i].pos = pt;
    sb->pts[sb->pts_i].vel = vec2(0, 0);

    return sb->pts_i++;
}

int softbody_add_spring(Softbody *sb, int i1, int i2)
{
    if (sb->springs_i >= sb->springs_siz)
    {
        sb->springs_siz *= 2;
        sb->springs = realloc(sb->springs, sizeof(Spring) * sb->springs_siz);
    }

    sb->springs[sb->springs_i].i1 = i1;
    sb->springs[sb->springs_i].i2 = i2;
    sb->springs[sb->springs_i].len = vec2_dist(sb->pts[i1].pos, sb->pts[i2].pos);

    return sb->springs_i++;
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

void softbody_update(Softbody *sb, Map *map)
{
    // Points
    for (int i = 0; i < sb->pts_i; ++i)
    {
        sb->pts[i].vel.y += GRAVITY;
        sb->pts[i].vel.y *= AIR_RESISTANCE;
        sb->pts[i].vel.x *= AIR_RESISTANCE;
        Vec2 next_pos = vec2_add(sb->pts[i].pos, sb->pts[i].vel);
        Vec2 pts_out[map->edges_i], ns_out[map->edges_i];
        int ints = map_raycast(map, sb->pts[i].pos, next_pos, pts_out, ns_out);
        for (int j = 0; j < ints; ++j)
        {
            sb->pts[i].vel = vec2_proj(sb->pts[i].vel, ns_out[j]);
        }

        // int next_pt_i = i == sb->pts_i - 1 ? 0 : i + 1;
        // Vec2 next_pt_next_pos = vec2_add(sb->pts[next_pt_i].pos, sb->pts[next_pt_i].vel);
        // if (map_raycast(map, next_pos, next_pt_next_pos, pts_out, ns_out))
        // {
        //     Vec2 next_pt_normal = vec2_sub(sb->pts[i].pos, sb->pts[next_pt_i].pos);
        //     sb->pts[i].vel = vec2_proj(sb->pts[i].vel, next_pt_normal);
        //     sb->pts[next_pt_i].vel = vec2_proj(sb->pts[next_pt_i].vel, next_pt_normal);
        // }

        sb->pts[i].pos = vec2_add(sb->pts[i].pos, sb->pts[i].vel);
    }

    // Springs
    for (int i = 0; i < sb->springs_i; ++i)
    {
        Point *p1 = &sb->pts[sb->springs[i].i1];
        Point *p2 = &sb->pts[sb->springs[i].i2];
        float dist = vec2_dist(p1->pos, p2->pos);
        Vec2 dir = vec2_normalized(vec2_sub(p1->pos, p2->pos));
        Vec2 f = vec2_mulf(dir, (dist - sb->springs[i].len) * SPRING_STRENGTH);
        p1->vel = vec2_sub(p1->vel, f);
        p2->vel = vec2_add(p2->vel, f);
    }
}

void softbody_render(Softbody *sb, Renderer *r)
{
    // debug render
    // SDL_SetRenderDrawColor(r->renderer, 0, 255, 255, 255);
    // for (int i = 0; i < sb->pts_i; ++i)
    // {
    //     renderer_draw_filled_circle(r, sb->pts[i].pos, 5);
    // }
    // SDL_SetRenderDrawColor(r->renderer, 255, 0, 0, 255);
    // for (int i = 0; i < sb->springs_i; ++i)
    // {
    //     Vec2 p1 = sb->pts[sb->springs[i].i1].pos;
    //     Vec2 p2 = sb->pts[sb->springs[i].i2].pos;
    //     renderer_draw_line(r, p1, p2);
    // }
    // SDL_SetRenderDrawColor(r->renderer, 255, 255, 255, 255);
    // renderer_draw_filled_circle(r, softbody_center(sb), 5);
    
    Vec2 c = softbody_center(sb);
    for (int i = 0; i < sb->pts_i; ++i)
    {
        int j = i == sb->pts_i - 1 ? 0 : i + 1;
        SDL_SetRenderDrawColor(r->renderer, 200, 200, 200, 255);
        renderer_draw_filled_solid_triangle(r, sb->pts[i].pos, sb->pts[j].pos, c);
        SDL_SetRenderDrawColor(r->renderer, 255, 100, 100, 255);
        renderer_draw_line(r, sb->pts[i].pos, sb->pts[j].pos);
    }
}


Softbody *softbody_create_rect(Vec2 pos, Vec2 siz)
{
    Softbody *sb = softbody_create();
    softbody_add_pt(sb, pos);
    softbody_add_pt(sb, vec2(pos.x + siz.x, pos.y));
    softbody_add_pt(sb, vec2(pos.x + siz.x, pos.y + siz.y));
    softbody_add_pt(sb, vec2(pos.x, pos.y + siz.y));
    softbody_connect_pts(sb);

    return sb;
}

Softbody *softbody_create_circle(Vec2 pos, float r, int num_pts)
{
    Softbody *sb = softbody_create();

    float da = 2 * M_PI / num_pts;

    for (int i = 0; i < num_pts; ++i)
    {
        float a = da * i;
        Vec2 pt = vec2_add(pos, vec2(cosf(a) * r, sinf(a) * r));
        softbody_add_pt(sb, pt);
    }
    softbody_connect_pts(sb);

    // SDL_Log("pts_i=%d, pts_siz=%d", sb->pts_i, sb->pts_siz);
    // SDL_Log("springs_i=%d, springs_siz=%d", sb->springs_i, sb->springs_siz);

    return sb;
}

Vec2 softbody_center(Softbody *sb)
{
    Vec2 c = vec2(0, 0);
    for (int i = 0; i < sb->pts_i; ++i)
    {
        c.x += sb->pts[i].pos.x;
        c.y += sb->pts[i].pos.y;
    }
    return vec2_divf(c, sb->pts_i);
}


void softbody_apply_force(Softbody *sb, Vec2 f)
{
    for (int i = 0; i < sb->pts_i; ++i)
    {
        sb->pts[i].vel = vec2_add(sb->pts[i].vel, f);
    }
}

void softbody_apply_force_closest(Softbody *sb, Vec2 pos, Vec2 f)
{
    float closest_dist = INFINITY;
    int closest_i;
    for (int i = 0; i < sb->pts_i; ++i)
    {
        float dist = vec2_dist_squared(sb->pts[i].pos, pos);
        if (dist < closest_dist)
        {
            closest_dist = dist;
            closest_i = i;
        }
    }
    sb->pts[closest_i].vel = vec2_add(sb->pts[closest_i].vel, f);
}


