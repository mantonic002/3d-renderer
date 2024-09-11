#include "vertex_shader.h"

void bind_world(VertexShader* shader, Mat transformation) {
    shader->world = transformation;
    if (shader->view.size == 4) {
        shader->world_view = multiply_matrices(shader->world, shader->view);
        
        if (shader->proj.size == 4) {
            shader->world_view_proj = multiply_matrices(shader->world_view, shader->proj);
        }
    }
}

void bind_world_view(VertexShader* shader, Mat transformation) {
    shader->world_view = transformation;
    if (shader->proj.size == 4) {
        shader->world_view_proj = multiply_matrices(shader->world_view, shader->proj);
    }
}

void bind_view(VertexShader* shader, Mat transformation) {
    shader->view = transformation;
    if (shader->world.size == 4) {
        shader->world_view = multiply_matrices(shader->world, shader->view);
        
        if (shader->proj.size == 4) {
            shader->world_view_proj = multiply_matrices(shader->world_view, shader->proj);
        }
    }
}

void bind_projection(VertexShader* shader, Mat transformation) {
    shader->proj = transformation;
    if (shader->world_view.size == 4) {
        shader->world_view_proj = multiply_matrices(shader->world_view, shader->proj);
    }
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
    Vec4 pos_v4_proj = multiply_matrix_by_vec4(shader->world_view_proj, &pos_v4);

    Vertex vertexOut;
    vertexOut.pos.as_vec4 = pos_v4_proj;
    vertexOut.tc = in->tc;
    vertexOut.n = multiply_matrix_by_vec3(shader->world_view, &in->n);
    vertexOut.col = in->col;

    Vec4 world_pos = multiply_matrix_by_vec4(shader->world, &pos_v4);
    vertexOut.world_pos = vec3(&world_pos);

    return vertexOut;
}