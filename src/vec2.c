#include "vec2.h"

Vec2 vec2_interpolate (const Vec2* v1, const Vec2* v2, const float alpha) {
    Vec2 result = {
        v1->x + (v2->x - v1->x) * alpha,
        v1->y + (v2->y - v1->y) * alpha,
    };
    return result;
}

Vec2 vec2_subtract (const Vec2* v1, const Vec2* v2) {
    Vec2 result = { 
        v1->x - v2->x,
        v1->y - v2->y,
    };
    return result;
}

Vec2 vec2_add (const Vec2* v1, const Vec2* v2) {
    Vec2 result = { 
        v1->x + v2->x,
        v1->y + v2->y,
    };
    return result;
}

Vec2 vec2_divide (const Vec2* v, float scalar) {
    Vec2 result = { 
        v->x/scalar,
        v->y/scalar,
    };
    return result;
}

Vec2 vec2_multiply (const Vec2* v, float scalar) {
    Vec2 result = { 
        v->x*scalar,
        v->y*scalar,
    };
    return result;
}

Vec2 convert_tex_coord(float u, float v) {
    Vec2 res = {
        (u + 1.0f) / 3.0f,
        v / 4.0f,
    };
    return res;
}