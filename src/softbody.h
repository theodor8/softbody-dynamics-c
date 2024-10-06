#pragma once

#include "vec2.h"
#include "renderer.h"

typedef struct Softbody Softbody;



Softbody *softbody_create(void);
void softbody_destroy(Softbody *sb);

// returns index
int softbody_add_pt(Softbody *sb, Vec2 pt);
int softbody_add_spring(Softbody *sb, int i1, int i2);

void softbody_connect_pts(Softbody *sb);

void softbody_update(Softbody *sb);

void softbody_render(Softbody *sb, Renderer *r);

Softbody *softbody_create_rect(Vec2 pos, Vec2 siz);
Softbody *softbody_create_circle(Vec2 pos, float r, int num_pts);

Vec2 softbody_center(Softbody *sb);

void softbody_apply_force(Softbody *sb, Vec2 f);
void softbody_apply_force_closest(Softbody *sb, Vec2 pos, Vec2 f);

