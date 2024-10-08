#pragma once

#include <stdbool.h>

#include "vec2.h"
#include "renderer.h"

typedef struct
{
    Vec2 p1, p2;
} Edge;

typedef struct
{
    Edge *edges;
    int edges_siz;
    int edges_i;
} Map;

Map *map_create(void);
void map_destroy(Map *m);

int map_add_edge(Map *m, Vec2 p1, Vec2 p2);

void map_render(Map *m, Renderer *r);

// intersect points, edge dirs (normals), returns number of intersections
int map_raycast(Map *m, Vec2 p1, Vec2 p2, Vec2 pts_out[], Vec2 ns_out[]);

