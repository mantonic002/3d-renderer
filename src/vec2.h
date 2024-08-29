#ifndef VEC2_H
#define VEC2_H

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Vec2 vec2_interpolate (const Vec2* v1, const Vec2* v2, const float alpha);
Vec2 vec2_subtract (const Vec2* v1, const Vec2* v2);
Vec2 vec2_add (const Vec2* v1, const Vec2* v2);
Vec2 vec2_divide (const Vec2* v, float scalar);
Vec2 vec2_multiply (const Vec2* v, float scalar);
Vec2 convert_tex_coord(float u, float v);

#endif