#include "vec2.h"

#include <math.h>


Vec2 vec2(float x, float y) { return (Vec2){x, y}; }
Vec2 vec2_add(Vec2 v1, Vec2 v2) { return vec2(v1.x + v2.x, v1.y + v2.y); }
Vec2 vec2_sub(Vec2 v1, Vec2 v2) { return vec2(v1.x - v2.x, v1.y - v2.y); }
float vec2_mul(Vec2 v1, Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

Vec2 vec2_addf(Vec2 v, float f) { return vec2(v.x + f, v.y + f); }
Vec2 vec2_subf(Vec2 v, float f) { return vec2(v.x - f, v.y - f); }
Vec2 vec2_mulf(Vec2 v, float f) { return vec2(v.x * f, v.y * f); }
Vec2 vec2_divf(Vec2 v, float f) { return vec2(v.x / f, v.y / f); }

Vec2 vec2_rot(Vec2 v, float a) { return vec2(cosf(a) * v.x - sinf(a) * v.y, sinf(a) * v.x + cosf(a) * v.y); }
Vec2 vec2_normalized(Vec2 v) { return vec2_divf(v, vec2_len(v)); }
Vec2 vec2_from_polar(float a, float l) { return vec2_mulf(vec2(cosf(a), sinf(a)), l); }
Vec2 vec2_proj(Vec2 v1, Vec2 v2) { return vec2_mulf(v2, vec2_mul(v1, v2) / vec2_len_squared(v2)); }

float vec2_len(Vec2 v) { return sqrtf(v.x * v.x + v.y * v.y); }
float vec2_len_squared(Vec2 v) { return v.x * v.x + v.y * v.y; }
float vec2_dist(Vec2 v1, Vec2 v2) { return vec2_len(vec2_sub(v1, v2)); }
float vec2_dist_squared(Vec2 v1, Vec2 v2) { return vec2_len_squared(vec2_sub(v1, v2)); }
float vec2_ang(Vec2 v) { return atanf(v.y / v.x); }

