#include "vertex_shader.h"

VertexShader* create_default_vertex_shader() {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->apply = default_vertex_shader_apply;
    }
    return shader;
}

Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in) {    
    Vec3 multiplied = multiply_matrix_by_point(shader->rotation, &in->pos);
    Vertex vertexOut = {
        vec3_add(&multiplied, &shader->translation),
        in->tc,
        in->col,
        in->n
    };
    return vertexOut;
}