#include "vertex.h"

Vertex vertex_interpolate (const Vertex* v1, const Vertex* v2, const float alpha) {
    Vertex result = {
        vec3_interpolate(&v1->pos, &v2->pos, alpha),
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
        vec3_interpolate(&v1->col, &v2->col, alpha),
        vec3_interpolate(&v1->n, &v2->n, alpha),
        v1->world_pos,
    };
    return result;
}

Vertex vertex_subtract (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        vec3_subtract(&v1->pos, &v2->pos),
        vec2_subtract(&v1->tc, &v2->tc),
        vec3_subtract(&v1->col, &v2->col),
        vec3_subtract(&v1->n, &v2->n),
        v1->world_pos,
    };
    return result;
}

Vertex vertex_add (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        vec3_add(&v1->pos, &v2->pos),
        vec2_add(&v1->tc, &v2->tc),
        vec3_add(&v1->col, &v2->col),
        vec3_add(&v1->n, &v2->n),
        v1->world_pos,
    };
    return result;
}

Vertex vertex_divide (const Vertex* v, float scalar) {
    Vertex result = {
        vec3_divide(&v->pos, scalar),
        vec2_divide(&v->tc, scalar),
        vec3_divide(&v->col, scalar),
        vec3_divide(&v->n, scalar),
        v->world_pos,
    };
    return result;
}

Vertex vertex_multiply (const Vertex* v, float scalar) {
    Vertex result = {
        vec3_multiply(&v->pos, scalar),
        vec2_multiply(&v->tc, scalar),
        vec3_multiply(&v->col, scalar),
        vec3_multiply(&v->n, scalar),
        v->world_pos,
    };
    return result;
}