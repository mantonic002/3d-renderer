#include "vertex_shader.h"

// constants for wave effect
float freqWave = 10.0f;
float freqScroll = 5.0f;
float amplitude = 0.05f;

VertexShader* create_wave_vertex_shader(float time) {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->time = time;
        shader->apply = wave_vertex_shader_apply;
    }
    return shader;
}

Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in) {    
    Vec4 pos_v4 = vec4(&in->pos.as_vec3, 1.0f);

    Vec4 pos_v4_proj = multiply_matrix_by_vec4(shader->world_view_proj, &pos_v4);
    pos_v4_proj.y += amplitude * sin( shader->time * freqScroll + pos_v4_proj.x * freqWave );

    Vertex vertexOut;
    vertexOut.pos.as_vec4 = pos_v4_proj;
    vertexOut.tc = in->tc;
    vertexOut.n = multiply_matrix_by_vec3(shader->world_view, &in->n);
    vertexOut.col = in->col;

    Vec4 world_pos = multiply_matrix_by_vec4(shader->world, &pos_v4);
    vertexOut.world_pos = vec3(&world_pos);

    return vertexOut;
}
        
