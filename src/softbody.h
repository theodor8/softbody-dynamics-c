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

void softbody_render(Softbody *sb, Renderer *r);


