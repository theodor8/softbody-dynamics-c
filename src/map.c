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

