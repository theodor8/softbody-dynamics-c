#include "softbody.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

#include "common.h"
#include "vec2.h"
#include "renderer.h"
#include "map.h"

struct Point
{
    Vec2 pos, vel;
};

struct Spring
{
    Point *pt1, *pt2;
    float len; // original length
};

struct Softbody
{
    Point **pts; // array of pointers to points
    int pts_siz;
    int pts_i;

    Spring **springs; // array of pointers to springs
    int springs_siz;
    int springs_i;

    float spring_strength;
};




static Point *point_create(Vec2 pos, Vec2 vel)
{
    Point *p = calloc(1, sizeof(Point));
    p->pos = pos;
    p->vel = vel;
    return p;
}

static void point_destroy(Point *p)
{
    free(p);
}


static Spring *spring_create(Point *pt1, Point *pt2)
{
    Spring *s = calloc(1, sizeof(Spring));
    s->pt1 = pt1;
    s->pt2 = pt2;
    s->len = vec2_dist(pt1->pos, pt2->pos);
    return s;
}

static void spring_destroy(Spring *s)
{
    free(s);
}


Softbody *sb_create(float spring_strength)
{
    Softbody *sb = calloc(1, sizeof(Softbody));

    sb->pts_siz = 10;
    sb->pts = calloc(sb->pts_siz, sizeof(Point *));

    sb->springs_siz = 10;
    sb->springs = calloc(sb->springs_siz, sizeof(Spring *));

    sb->spring_strength = spring_strength;

    return sb;
}

void sb_destroy(Softbody *sb)
{
    for (int i = 0; i < sb->pts_i; ++i)
    {
        point_destroy(sb->pts[i]);
    }
    free(sb->pts);

    for (int i = 0; i < sb->springs_i; ++i)
    {
        spring_destroy(sb->springs[i]);
    }
    free(sb->springs);

    free(sb);
}

Point *sb_add_pt(Softbody *sb, Vec2 pos)
{
    if (sb->pts_i >= sb->pts_siz)
    {
        sb->pts_siz *= 2;
        sb->pts = realloc(sb->pts, sizeof(Point *) * sb->pts_siz);
    }

    Point *point = point_create(pos, vec2(0, 0));
    sb->pts[sb->pts_i] = point;

    ++sb->pts_i;

    return point;
}

Spring *sb_add_spring(Softbody *sb, Point *pt1, Point *pt2)
{
    if (sb->springs_i >= sb->springs_siz)
    {
        sb->springs_siz *= 2;
        sb->springs = realloc(sb->springs, sizeof(Spring *) * sb->springs_siz);
    }

    Spring *spring = spring_create(pt1, pt2);
    sb->springs[sb->springs_i] = spring;

    ++sb->springs_i;

    return spring;
}

void sb_connect_pts(Softbody *sb)
{
    for (int i1 = 0; i1 < sb->pts_i; ++i1)
    {
        for (int i2 = i1 + 1; i2 < sb->pts_i; ++i2)
        {
            sb_add_spring(sb, sb->pts[i1], sb->pts[i2]);
        }
    }
}

void sb_update(Softbody *sb, Map *map)
{
    // Points
    for (int i = 0; i < sb->pts_i; ++i)
    {
        Point *point = sb->pts[i];

        point->vel.x *= 1.0f - AIR_RESISTANCE;
        point->vel.y *= 1.0f - AIR_RESISTANCE;
        point->vel.y += GRAVITY;

        Vec2 next_pos = vec2_add(point->pos, point->vel);
        Vec2 pts_out[map->edges_i]; // intersection points
        Edge *edges_out[map->edges_i]; // intersections edges
        int intersections = map_raycast(map, point->pos, next_pos, pts_out, edges_out);
        
        // Project velocity on wall direction --> velocity efter collission
        // Do it for every intersecting wall
        for (int j = 0; j < intersections; ++j)
        {
            Vec2 edge_dir = vec2_sub(edges_out[j]->p1, edges_out[j]->p2);
            point->vel.x *= 1.0f - FRICTION;
            point->vel.y *= 1.0f - FRICTION;
            point->vel = vec2_proj(point->vel, edge_dir);
        }
    }

    for (int i = 0; i < sb->pts_i; ++i)
    {
        Point *point = sb->pts[i];
        point->pos = vec2_add(point->pos, point->vel);
    }
    

    // Springs
    for (int i = 0; i < sb->springs_i; ++i)
    {
        Spring *spring = sb->springs[i];
        Point *pt1 = spring->pt1;
        Point *pt2 = spring->pt2;

        float dist = vec2_dist(pt1->pos, pt2->pos);
        Vec2 dir = vec2_normalized(vec2_sub(pt1->pos, pt2->pos));
        Vec2 f = vec2_mulf(dir, (dist - spring->len) * sb->spring_strength);
        pt1->vel = vec2_sub(pt1->vel, f);
        pt2->vel = vec2_add(pt2->vel, f);
    }
}

void sb_render(Softbody *sb, Renderer *r)
{
    // Debug render
    for (int i = 0; i < sb->springs_i; ++i)
    {
        Spring *spring = sb->springs[i];
        Vec2 pos1 = spring->pt1->pos;
        Vec2 pos2 = spring->pt2->pos;
        float dist = vec2_dist(pos1, pos2);

        int color = (dist / spring->len -1) * 2 * 255;
        color = color < 0 ? -color : color;
        color = color > 255 ? 255 : color;

        SDL_SetRenderDrawColor(r->renderer, color, 255 - color, 100, 255);
        renderer_draw_line(r, pos1, pos2);
    }
    SDL_SetRenderDrawColor(r->renderer, 100, 255, 255, 200);
    for (int i = 0; i < sb->pts_i; ++i)
    {        
        renderer_draw_filled_circle(r, sb->pts[i]->pos, 3);
    }
}


Softbody *sb_create_rect(Vec2 pos, Vec2 siz, float spring_strength)
{
    Softbody *sb = sb_create(spring_strength);
    sb_add_pt(sb, pos);
    sb_add_pt(sb, vec2(pos.x + siz.x, pos.y));
    sb_add_pt(sb, vec2(pos.x + siz.x, pos.y + siz.y));
    sb_add_pt(sb, vec2(pos.x, pos.y + siz.y));
    sb_connect_pts(sb);

    return sb;
}

Softbody *sb_create_filled_rect(Vec2 pos, Vec2 siz, int rows, int cols, float spring_strength)
{
    Softbody *sb = sb_create(spring_strength);

    siz.x /= cols;
    siz.y /= rows;
    for (int r = 0; r < rows; ++r)
    {
        float y = pos.y + siz.y * r;
        for (int c = 0; c < cols; ++c)
        {
            float x = pos.x + siz.x * c;
            sb_add_pt(sb, vec2(x, y));
            
        }
    }

    // Connect points
    for (int i = 0; i < sb->pts_i; ++i)
    {
        Point *pt = sb->pts[i];
        if (i % cols != 0) // Left
        {
            sb_add_spring(sb, pt, sb->pts[i - 1]);
        }
        if (i >= cols) // Up
        {
            sb_add_spring(sb, pt, sb->pts[i - cols]);
        }
        if (i % cols != 0 && i >= cols) // Left Up
        {
            sb_add_spring(sb, pt, sb->pts[i - cols - 1]);
        }
        if (i % cols != 0 && i < sb->pts_i - cols) // Left Down
        {
            sb_add_spring(sb, pt, sb->pts[i + cols - 1]);
        }
    }

    return sb;
}

Softbody *sb_create_circle(Vec2 pos, float r, int num_pts, float spring_strength)
{
    Softbody *sb = sb_create(spring_strength);

    float da = 2 * M_PI / num_pts;

    for (int i = 0; i < num_pts; ++i)
    {
        float a = da * i;
        Vec2 pt = vec2_add(pos, vec2(cosf(a) * r, sinf(a) * r));
        sb_add_pt(sb, pt);
    }
    sb_connect_pts(sb);

    // SDL_Log("pts_i=%d, pts_siz=%d", sb->pts_i, sb->pts_siz);
    // SDL_Log("springs_i=%d, springs_siz=%d", sb->springs_i, sb->springs_siz);

    return sb;
}

Vec2 sb_center(Softbody *sb)
{
    Vec2 c = vec2(0, 0);
    for (int i = 0; i < sb->pts_i; ++i)
    {
        c.x += sb->pts[i]->pos.x;
        c.y += sb->pts[i]->pos.y;
    }
    return vec2_divf(c, sb->pts_i);
}


void sb_apply_force(Softbody *sb, Vec2 f)
{
    for (int i = 0; i < sb->pts_i; ++i)
    {
        sb->pts[i]->vel = vec2_add(sb->pts[i]->vel, f);
    }
}

void sb_apply_force_closest(Softbody *sb, Vec2 pos, Vec2 f)
{
    float closest_dist = INFINITY;
    int closest_i;
    for (int i = 0; i < sb->pts_i; ++i)
    {
        float dist = vec2_dist_squared(sb->pts[i]->pos, pos);
        if (dist < closest_dist)
        {
            closest_dist = dist;
            closest_i = i;
        }
    }
    sb->pts[closest_i]->vel = vec2_add(sb->pts[closest_i]->vel, f);
}


