#ifndef TEX_VER_H
#define TEX_VER_H

#include "vec2.h"

typedef struct TexVertex
{
    Vec2 pos; // position
    Vec2 tc; // texture coordinate
} TexVertex;

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha);

#endif