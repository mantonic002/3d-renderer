#include "vertex_shader.h"

VertexShader* create_wave_vertex_shader(const float rotation[3][3], const Vec3* translation, float time) {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        memcpy(shader->rotation, rotation, 3 * 3 * sizeof(float));
        shader->translation = *translation;
        shader->time = time;
        shader->apply = wave_vertex_shader_apply;
    }
    return shader;
}

Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in) {    
    Vec3 multiplied;
    
    multiply_matrix_by_point(shader->rotation, &in->pos, &multiplied);
    Vec3 pos = vec3_add(&multiplied, &shader->translation);
    pos.y += amplitude * sin( shader->time * freqScroll + pos.x * freqWave );
    
    Vertex vertexOut = {
        pos,
        in->tc,
    };
    return vertexOut;
}
        
