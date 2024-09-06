#include "vertex_shader.h"

// constants for wave effect
float freqWave = 10.0f;
float freqScroll = 5.0f;
float amplitude = 0.05f;

VertexShader* create_wave_vertex_shader(const Mat rotation, const Vec3* translation, float time) {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        memcpy(&shader->rotation, &rotation, sizeof(Mat));
        shader->translation = *translation;
        shader->time = time;
        shader->apply = wave_vertex_shader_apply;
    }
    return shader;
}

Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in) {    
    Vec3 multiplied = multiply_matrix_by_vec3(shader->rotation, &in->pos.as_vec3);
    Vec3 pos = vec3_add(&multiplied, &shader->translation);
    pos.y += amplitude * sin( shader->time * freqScroll + pos.x * freqWave );
    
    Vertex vertexOut = {
        {.as_vec3 = pos},
        in->tc,
        in->col,
        in->n
    };
    return vertexOut;
}
        
