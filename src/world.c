#include "world.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

#include "vec2.h"
#include "renderer.h"
#include "softbody.h"


struct Edge
{
    Vec2 p1, p2;
};

struct World
{
    float gravity, drag, friction;

    Point **pts; // array of pointers to points
    int pts_siz;
    int pts_i;

    Spring **springs; // array of pointers to springs
    int springs_siz;
    int springs_i;

    Edge **edges;
    int edges_siz;
    int edges_i;
};



static Edge *edge_create(Vec2 p1, Vec2 p2)
{
    Edge *edge = calloc(1, sizeof(Edge));
    edge->p1 = p1;
    edge->p2 = p2;
    return edge;
}

static void edge_destroy(Edge *edge)
{
    free(edge);
}

static void edge_render(Renderer *renderer, Edge *edge)
{
    renderer_set_draw_color(renderer, 255, 100, 100, 255);
    renderer_draw_line(renderer, edge->p1, edge->p2);
}


// static int raycast(World *world, Vec2 p1, Vec2 p2, Vec2 pts_out[], Edge *edges_out[])
// {
//     // int out_i = 0;
//     // for (int i = 0; i < m->edges_i; ++i)
//     // {
//     //     if (line_intersection(p1, p2, m->edges[i].p1, m->edges[i].p2, &pts_out[out_i]))
//     //     {
//     //         edges_out[out_i] = &m->edges[i];
//     //         ++out_i;
//     //     }
//     // }
//     // return out_i;
//     return 0;
// }





World *world_create(float gravity, float drag, float friction)
{
    World *world = calloc(1, sizeof(World));

    world->gravity = gravity;
    world->drag = drag;
    world->friction = friction;

    world->pts_siz = 10;
    world->pts = calloc(world->pts_siz, sizeof(Point *));

    world->springs_siz = 10;
    world->springs = calloc(world->springs_siz, sizeof(Spring *));

    world->edges_siz = 10;
    world->edges = calloc(world->edges_siz, sizeof(Edge *));

    return world;
}

void world_destroy(World *world)
{
    for (int i = 0; i < world->pts_i; ++i)
    {
        point_destroy(world->pts[i]);
    }
    free(world->pts);

    for (int i = 0; i < world->springs_i; ++i)
    {
        spring_destroy(world->springs[i]);
    }
    free(world->springs);

    for (int i = 0; i < world->edges_i; ++i)
    {
        edge_destroy(world->edges[i]);
    }
    free(world->edges);

    free(world);
}

Point *world_add_pt(World *world, Vec2 pos)
{
    if (world->pts_i >= world->pts_siz)
    {
        world->pts_siz *= 2;
        world->pts = realloc(world->pts, sizeof(Point *) * world->pts_siz);
    }

    Point *point = point_create(pos, vec2(0, 0));
    world->pts[world->pts_i] = point;

    ++world->pts_i;

    return point;
}

Spring *world_add_spring(World *world, Point *pt1, Point *pt2, float strength, float damping)
{
    if (world->springs_i >= world->springs_siz)
    {
        world->springs_siz *= 2;
        world->springs = realloc(world->springs, sizeof(Spring *) * world->springs_siz);
    }

    Spring *spring = spring_create(pt1, pt2, strength, damping);
    world->springs[world->springs_i] = spring;

    ++world->springs_i;

    return spring;
}

Edge *world_add_edge(World *world, Vec2 p1, Vec2 p2)
{
    if (world->edges_i >= world->edges_siz)
    {
        world->edges_siz *= 2;
        world->edges = realloc(world->edges, sizeof(Edge) * world->edges_siz);
    }

    Edge *edge = edge_create(p1, p2);
    world->edges[world->edges_i] = edge;

    ++world->edges_i;

    return edge;
}

void world_step(World *world)
{

    for (int i = 0; i < world->springs_i; ++i)
    {
        spring_step(world->springs[i]);
    }

    for (int i = 0; i < world->pts_i; ++i)
    {
        point_apply_force(world->pts[i], vec2(0, world->gravity));
        point_apply_force(world->pts[i], vec2_mulf(point_get_vel(world->pts[i]), -world->drag));
        for (int j = 0; j < world->edges_i; ++j)
        {
            if (point_edge_collision(world->pts[i], world->edges[j]->p1, world->edges[j]->p2))
            {
                point_apply_force(world->pts[i], vec2_mulf(point_get_vel(world->pts[i]), -world->friction));
            }
        }

        for (int j = 0; j < world->springs_i; ++j)
        {
            point_spring_collision(world->pts[i], world->springs[j]);
        }
        
    }

    for (int i = 0; i < world->pts_i; ++i)
    {
        point_step(world->pts[i]);
    }


    
}

void world_render(World *world, Renderer *renderer)
{
    for (int i = 0; i < world->springs_i; ++i)
    {
        spring_render(world->springs[i], renderer);
    }

    for (int i = 0; i < world->pts_i; ++i)
    {
        point_render(world->pts[i], renderer);
    }

    for (int i = 0; i < world->edges_i; ++i)
    {
        edge_render(renderer, world->edges[i]);
    }
}





// Vec2 world_center(World *world)
// {
//     Vec2 c = vec2(0, 0);
//     for (int i = 0; i < world->pts_i; ++i)
//     {
//         c.x += world->pts[i]->pos.x;
//         c.y += world->pts[i]->pos.y;
//     }
//     return vec2_divf(c, world->pts_i);
// }


// void world_apply_force_all(World *world, Vec2 f)
// {
//     for (int i = 0; i < world->pts_i; ++i)
//     {
//         world->pts[i]->force = vec2_add(world->pts[i]->force, f);
//     }
// }

// void world_apply_force_closest(World *world, Vec2 pos, Vec2 f)
// {
//     float closest_dist = INFINITY;
//     int closest_i;
//     for (int i = 0; i < world->pts_i; ++i)
//     {
//         float dist = vec2_dist_squared(world->pts[i]->pos, pos);
//         if (dist < closest_dist)
//         {
//             closest_dist = dist;
//             closest_i = i;
//         }
//     }
//     world->pts[closest_i]->force = vec2_add(world->pts[closest_i]->force, f);
// }


