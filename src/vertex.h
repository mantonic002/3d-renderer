#ifndef TEX_VER_H
#define TEX_VER_H

#include "vec3.h"
#include "vec2.h"

typedef struct Vertex
{
    Vec3 pos; // position
    Vec2 tc; // texture coordinate
    Vec3 col;
    Vec3 n; // normal
} Vertex;

Vertex vertex_interpolate (const Vertex* v1, const Vertex* v2, const float alpha);
Vertex vertex_subtract (const Vertex* v1, const Vertex* v2);
Vertex vertex_add (const Vertex* v1, const Vertex* v2);
Vertex vertex_divide (const Vertex* v, float scalar);
Vertex vertex_multiply (const Vertex* v, float scalar);

#endif