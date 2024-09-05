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
    Vec3 pos = vec3_add(&multiplied, &shader->translation);

    Vertex vertexOut;
    vertexOut.pos = pos;
    vertexOut.tc = in->tc;
    vertexOut.n = multiply_matrix_by_point(shader->rotation, &in->n);
    vertexOut.col = in->col;
    vertexOut.world_pos = pos;

    return vertexOut;
}