#include "map.h"

#include "vec2.h"


Map *map_create(void)
{
    Map *m = calloc(1, sizeof(Map));

    m->edges_siz = 10;
    m->edges = calloc(m->edges_siz, sizeof(Edge));

    return m;
}

void map_destroy(Map *m)
{
    free(m->edges);
    free(m);
}

int map_add_edge(Map *m, Vec2 p1, Vec2 p2)
{
    if (m->edges_i >= m->edges_siz)
    {
        m->edges_siz *= 2;
        m->edges = realloc(m->edges, sizeof(Edge) * m->edges_siz);
    }

    m->edges[m->edges_i].p1 = p1;
    m->edges[m->edges_i].p2 = p2;

    return m->edges_i++;
}

void map_render(Map *m, Renderer *r)
{
    SDL_SetRenderDrawColor(r->renderer, 255, 100, 100, 255);
    for (int i = 0; i < m->edges_i; ++i)
    {
        renderer_draw_line(r, m->edges[i].p1, m->edges[i].p2);
    }
}

static bool line_intersection(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2, Vec2* out)
{
    float t = ((a1.x-b1.x) * (b1.y-b2.y) - (a1.y-b1.y) * (b1.x-b2.x)) / ((a1.x-a2.x) * (b1.y-b2.y) - (a1.y-a2.y) * (b1.x-b2.x));
    if (t < 0 || t > 1) return false;
    float u = -((a1.x-a2.x) * (a1.y-b1.y) - (a1.y-a2.y) * (a1.x-b1.x)) / ((a1.x-a2.x) * (b1.y-b2.y) - (a1.y-a2.y) * (b1.x-b2.x));
    if (u < 0 || u > 1) return false;
    *out = vec2(a1.x + t * (a2.x - a1.x), a1.y + t * (a2.y - a1.y));
    return true;
}

int map_raycast(Map *m, Vec2 p1, Vec2 p2, Vec2 pts_out[], Edge *edges_out[])
{
    int out_i = 0;
    for (int i = 0; i < m->edges_i; ++i)
    {
        if (line_intersection(p1, p2, m->edges[i].p1, m->edges[i].p2, &pts_out[out_i]))
        {
            edges_out[out_i] = &m->edges[i];
            ++out_i;
        }
    }
    return out_i;
}

