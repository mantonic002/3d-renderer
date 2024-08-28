#include "vertex_shader.h"

VertexShader* create_default_vertex_shader(const float rotation[3][3], const Vec3* translation) {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        memcpy(shader->rotation, rotation, 3 * 3 * sizeof(float));
        shader->translation = *translation;
        shader->apply = default_vertex_shader_apply;
    }
    return shader;
}

Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in) {    
    Vec3 multiplied;
    multiplyMatrixByPoint(shader->rotation, &in->pos, &multiplied);
    Vertex vertexOut = {
        vec3_add(&multiplied, &shader->translation),
        in->tc,
    };
    return vertexOut;
}