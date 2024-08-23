#ifndef TEX_VER_H
#define TEX_VER_H

#include "vec3.h"
#include "vec2.h"

typedef struct TexVertex
{
    Vec3 pos; // position
    Vec2 tc; // texture coordinate
} TexVertex;

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha);
TexVertex tex_vertex_subtract (const TexVertex* v1, const TexVertex* v2);
TexVertex tex_vertex_add (const TexVertex* v1, const TexVertex* v2);
TexVertex tex_vertex_divide (const TexVertex* v, float scalar);
TexVertex tex_vertex_multiply (const TexVertex* v, float scalar);

#endif