#include "vertex.h"

Vertex vertex_interpolate (const Vertex* v1, const Vertex* v2, const float alpha) {
    Vertex result = {
        {.as_vec4 = vec4_interpolate(&v1->pos.as_vec4, &v2->pos.as_vec4, alpha)},
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
        vec3_interpolate(&v1->col, &v2->col, alpha),
        vec3_interpolate(&v1->n, &v2->n, alpha),
        vec3_interpolate(&v1->world_pos, &v2->world_pos, alpha)
    };
    return result;
}

Vertex vertex_subtract (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        {.as_vec4 = vec4_subtract(&v1->pos.as_vec4, &v2->pos.as_vec4)},
        vec2_subtract(&v1->tc, &v2->tc),
        vec3_subtract(&v1->col, &v2->col),
        vec3_subtract(&v1->n, &v2->n),
        vec3_subtract(&v1->world_pos, &v2->world_pos)
    };
    return result;
}

Vertex vertex_add (const Vertex* v1, const Vertex* v2) {
    Vertex result = {
        {.as_vec4 = vec4_add(&v1->pos.as_vec4, &v2->pos.as_vec4)},
        vec2_add(&v1->tc, &v2->tc),
        vec3_add(&v1->col, &v2->col),
        vec3_add(&v1->n, &v2->n),
        vec3_add(&v1->world_pos, &v2->world_pos)
    };
    return result;
}

Vertex vertex_divide (const Vertex* v, float scalar) {
    Vertex result = {
        {.as_vec4 = vec4_divide(&v->pos.as_vec4, scalar)},
        vec2_divide(&v->tc, scalar),
        vec3_divide(&v->col, scalar),
        vec3_divide(&v->n, scalar),
        vec3_divide(&v->world_pos, scalar)

    };
    return result;
}

Vertex vertex_multiply (const Vertex* v, float scalar) {
    Vertex result = {
        {.as_vec4 = vec4_multiply(&v->pos.as_vec4, scalar)},
        vec2_multiply(&v->tc, scalar),
        vec3_multiply(&v->col, scalar),
        vec3_multiply(&v->n, scalar),
        vec3_multiply(&v->world_pos, scalar),
        
    };
    return result;
}