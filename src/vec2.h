#pragma once

#include <stdbool.h>

typedef struct
{
    float x, y;
} Vec2;

Vec2 vec2(float x, float y);

Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_sub(Vec2 v1, Vec2 v2);
float vec2_mul(Vec2 v1, Vec2 v2); // dot product

Vec2 vec2_addf(Vec2 v, float f);
Vec2 vec2_subf(Vec2 v, float f);
Vec2 vec2_mulf(Vec2 v, float f);
Vec2 vec2_divf(Vec2 v, float f);

Vec2 vec2_rot(Vec2 v, float a);
Vec2 vec2_normalized(Vec2 v);
Vec2 vec2_from_polar(float a, float l);
Vec2 vec2_proj(Vec2 v1, Vec2 v2); // v1 on v2
Vec2 vec2_reflect(Vec2 v1, Vec2 v2); // v1 on v2
Vec2 vec2_normal(Vec2 v);
Vec2 vec2_neg(Vec2 v);

float vec2_len(Vec2 v);
float vec2_len_squared(Vec2 v);
float vec2_dist(Vec2 v1, Vec2 v2);
float vec2_dist_squared(Vec2 v1, Vec2 v2);
float vec2_ang(Vec2 v);

bool vec2_equal(Vec2 v1, Vec2 v2);

bool line_intersection(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2, Vec2* out);

