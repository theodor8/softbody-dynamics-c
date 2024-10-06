#include "softbody.h"

#include <stdlib.h>

#include "vec2.h"

typedef struct
{
    int i1, i2; // indices to pts array
    float len; // original length
} Spring;


struct Softbody
{
    Vec2 *pts;
    size_t pts_siz;
};

