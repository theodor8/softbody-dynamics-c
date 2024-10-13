#pragma once

#include "vec2.h"
#include "renderer.h"
#include "softbody.h"


typedef struct Edge Edge;
typedef struct World World;



World *world_create(float gravity, float drag, float friction);
void world_destroy(World *world);

Point *world_add_pt(World *world, Vec2 pos);
Spring *world_add_spring(World *world, Point *pt1, Point *pt2, float strength, float damping);
Edge *world_add_edge(World *world, Vec2 p1, Vec2 p2);

void world_step(World *world);
void world_render(World *world, Renderer *renderer);



