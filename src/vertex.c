#include "vertex.h"

Vertex vertex_interpolate (const Vertex* v1, const Vertex* v2, const float alpha) {
    Vertex result = {
        vec3_interpolate(&v1->pos, &v2->pos, alpha),
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
    };
    return result;
}

Vertex vertex_subtract (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        vec3_subtract(&v1->pos, &v2->pos),
        vec2_subtract(&v1->tc, &v2->tc),
    };
    return result;
}

Vertex vertex_add (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        vec3_add(&v1->pos, &v2->pos),
        vec2_add(&v1->tc, &v2->tc),
    };
    return result;
}

Vertex vertex_divide (const Vertex* v, float scalar) {
    Vertex result = {
        vec3_divide(&v->pos, scalar),
        vec2_divide(&v->tc, scalar),
    };
    return result;
}

Vertex vertex_multiply (const Vertex* v, float scalar) {
    Vertex result = {
        vec3_multiply(&v->pos, scalar),
        vec2_multiply(&v->tc, scalar),
    };
    return result;
}