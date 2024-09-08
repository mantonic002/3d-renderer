#include "vertex_shader.h"

void bind_world(VertexShader* shader, Mat transformation) {
    shader->world = transformation;
    shader->worldProj = multiply_matrices(shader->world, shader->proj);
}

void bind_projection(VertexShader* shader, Mat transformation) {
    shader->proj = transformation;
    shader->worldProj = multiply_matrices(shader->world, shader->proj);
}


VertexShader* create_default_vertex_shader() {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->apply = default_vertex_shader_apply;
    }
    return shader;
}

Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in) {   
    Vec4 pos_v4 = vec4(&in->pos.as_vec3, 1.0f);
    Vec4 pos_v4_proj = multiply_matrix_by_vec4(shader->worldProj, &pos_v4);

    Vec4 world_pos = multiply_matrix_by_vec4(shader->world, &pos_v4);

    Vertex vertexOut;
    vertexOut.pos.as_vec4 = pos_v4_proj;
    vertexOut.tc = in->tc;
    vertexOut.n = multiply_matrix_by_vec3(shader->world, &in->n);
    vertexOut.col = in->col;
    vertexOut.world_pos = vec3(&world_pos);

    return vertexOut;
}