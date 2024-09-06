#ifndef TEX_VER_H
#define TEX_VER_H

#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

typedef union {
    Vec3 as_vec3;
    Vec4 as_vec4;
} VecUnion;

typedef struct Vertex
{
    VecUnion pos; // position as both Vec3 and Vec4
    Vec2 tc; // texture coordinate
    Vec3 col;
    Vec3 n; // normal
    Vec3 world_pos; // real world position that's not getting changed by the pipeline
} Vertex;

Vertex vertex_interpolate (const Vertex* v1, const Vertex* v2, const float alpha);
Vertex vertex_subtract (const Vertex* v1, const Vertex* v2);
Vertex vertex_add (const Vertex* v1, const Vertex* v2);
Vertex vertex_divide (const Vertex* v, float scalar);
Vertex vertex_multiply (const Vertex* v, float scalar);

#endif