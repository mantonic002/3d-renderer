#include "tex_vertex.h"

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha) {
    TexVertex result = {
        vec3_interpolate(&v1->pos, &v2->pos, alpha),
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
    };
    return result;
}

TexVertex tex_vertex_subtract (const TexVertex* v1, const TexVertex* v2) {
    TexVertex result = {
        vec3_subtract(&v1->pos, &v2->pos),
        vec2_subtract(&v1->tc, &v2->tc),
    };
    return result;
}

TexVertex tex_vertex_add (const TexVertex* v1, const TexVertex* v2) {
    TexVertex result = {
        vec3_add(&v1->pos, &v2->pos),
        vec2_add(&v1->tc, &v2->tc),
    };
    return result;
}

TexVertex tex_vertex_divide (const TexVertex* v, float scalar) {
    TexVertex result = {
        vec3_divide(&v->pos, scalar),
        vec2_divide(&v->tc, scalar),
    };
    return result;
}

TexVertex tex_vertex_multiply (const TexVertex* v, float scalar) {
    TexVertex result = {
        vec3_multiply(&v->pos, scalar),
        vec2_multiply(&v->tc, scalar),
    };
    return result;
}