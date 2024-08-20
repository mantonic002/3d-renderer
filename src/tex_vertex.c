#include "tex_vertex.h"

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha) {
    TexVertex result = {
        vec2_interpolate(&v1->pos, &v2->pos, alpha),
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
    };
    return result;
}
