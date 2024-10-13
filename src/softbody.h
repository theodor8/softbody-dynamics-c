#pragma once

#include "vec2.h"
#include "renderer.h"
#include "map.h"

typedef struct Point Point;
typedef struct Spring Spring;
typedef struct Softbody Softbody;



Softbody *sb_create(void);
void sb_destroy(Softbody *sb);

Point *sb_add_pt(Softbody *sb, Vec2 pos);
Spring *sb_add_spring(Softbody *sb, Point *pt1, Point *pt2, float strength, float damping);

void sb_connect_pts(Softbody *sb, float spring_strength, float spring_damping);

void sb_update(Softbody *sb, Map *map);

void sb_render(Softbody *sb, Renderer *r);

Softbody *sb_create_rect(Vec2 pos, Vec2 siz, float spring_strength, float spring_damping);
Softbody *sb_create_filled_rect(Vec2 pos, Vec2 siz, int rows, int cols, float spring_strength, float spring_damping);
Softbody *sb_create_circle(Vec2 pos, float r, int num_pts, float spring_strength, float spring_damping);

Vec2 sb_center(Softbody *sb);

void sb_apply_force_all(Softbody *sb, Vec2 f);
void sb_apply_force_closest(Softbody *sb, Vec2 pos, Vec2 f);

