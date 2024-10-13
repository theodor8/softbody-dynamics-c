#pragma once

#include "vec2.h"
#include "renderer.h"

typedef struct Point Point;
typedef struct Spring Spring;

Vec2 point_get_pos(Point *point);
Vec2 point_get_vel(Point *point);
Point *point_create(Vec2 pos, Vec2 vel);
void point_destroy(Point *point);
void point_step(Point *point);
void point_render(Point *point, Renderer *renderer);
void point_apply_force(Point *point, Vec2 force);
bool point_edge_collision(Point *point, Vec2 p1, Vec2 p2);
void point_spring_collision(Point *point, Spring *spring);

Spring *spring_create(Point *pt1, Point *pt2, float strength, float damping);
void spring_destroy(Spring *spring);
void spring_step(Spring *spring);
void spring_render(Spring *spring, Renderer *renderer);
