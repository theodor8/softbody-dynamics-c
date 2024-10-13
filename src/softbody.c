#include "softbody.h"

#include "vec2.h"


struct Point
{
    Vec2 pos, vel, force;
    float mass;
};

struct Spring
{
    Point *pt1, *pt2;
    float length, strength, damping;
};



Vec2 point_get_pos(Point *point)
{
    return point->pos;
}
Vec2 point_get_vel(Point *point)
{
    return point->vel;
}

Point *point_create(Vec2 pos, Vec2 vel)
{
    Point *p = calloc(1, sizeof(Point));
    p->pos = pos;
    p->vel = vel;
    p->mass = 1;
    return p;
}

void point_destroy(Point *point)
{
    free(point);
}

void point_step(Point *point)
{
    point->vel = vec2_add(point->vel, point->force);
    point->pos = vec2_add(point->pos, point->vel);
    point->force = vec2(0, 0);
}

void point_render(Point *point, Renderer *renderer)
{
    renderer_set_draw_color(renderer, 100, 255, 255, 200);
    renderer_draw_filled_circle(renderer, point->pos, 3);
}

void point_apply_force(Point *point, Vec2 force)
{
    point->force = vec2_add(point->force, force);
}

bool point_edge_collision(Point *point, Vec2 p1, Vec2 p2)
{
    Vec2 next_vel = vec2_add(point->vel, point->force);
    Vec2 next_pos = vec2_add(point->pos, next_vel);
    Vec2 intersection_point;
    if (line_intersection(point->pos, next_pos, p1, p2, &intersection_point))
    {
        // Project velocity on wall direction --> velocity efter collission
        Vec2 edge_dir = vec2_sub(p1, p2);
        point->vel = vec2_proj(point->vel, edge_dir);
        point->force = vec2_proj(point->force, edge_dir);
        return true;
    }
    return false;

}

void point_spring_collision(Point *point, Spring *spring)
{
    Vec2 next_vel = vec2_add(point->vel, point->force);
    Vec2 next_pos = vec2_add(point->pos, next_vel);
    Vec2 intersection_point;
    if (line_intersection(point->pos, next_pos, spring->pt1->pos, spring->pt2->pos, &intersection_point))
    {
        // Vec2 spring_pt_vel = vec2_divf(vec2_add(spring->pt1->vel, spring->pt2->vel), 2);
        // TODO
    }
}




Spring *spring_create(Point *pt1, Point *pt2, float strength, float damping)
{
    Spring *s = calloc(1, sizeof(Spring));
    s->pt1 = pt1;
    s->pt2 = pt2;
    s->length = vec2_dist(pt1->pos, pt2->pos);
    s->strength = strength;
    s->damping = damping;
    return s;
}

void spring_destroy(Spring *spring)
{
    free(spring);
}

void spring_step(Spring *spring)
{
    Point *pt1 = spring->pt1;
    Point *pt2 = spring->pt2;

    float dist = vec2_dist(pt1->pos, pt2->pos);
    Vec2 dir = vec2_normalized(vec2_sub(pt2->pos, pt1->pos));

    float spring_force = (dist - spring->length) * spring->strength;
    float spring_damping = vec2_mul(dir, vec2_sub(pt2->vel, pt1->vel)) * spring->damping;

    Vec2 force_vec = vec2_mulf(dir, spring_force + spring_damping);
    point_apply_force(pt1, force_vec);
    point_apply_force(pt2, vec2_neg(force_vec));
}

void spring_render(Spring *spring, Renderer *renderer)
{
    Vec2 pos1 = spring->pt1->pos;
    Vec2 pos2 = spring->pt2->pos;
    float dist = vec2_dist(pos1, pos2);

    int color = (dist / spring->length - 1) * 2 * 255;
    color = color < 0 ? -color : color;
    color = color > 255 ? 255 : color;

    renderer_set_draw_color(renderer, color, 255 - color, 100, 255);
    renderer_draw_line(renderer, pos1, pos2);
}


