#include "vertex_shader.h"

Vec3 dir = { 0.0f,0.0f,1.0f };
Vec3 diffuse = { 1.0f,1.0f,1.0f };
Vec3 ambient = { 0.1f,0.1f,0.1f };
Vec3 color = { 0.8f,0.85f,1.0f };

VertexShader* create_flat_shading_vertex_shader(const float rotation[3][3], const Vec3* translation) {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        memcpy(shader->rotation, rotation, 3 * 3 * sizeof(float));
        shader->translation = *translation;
        shader->apply = flat_shading_vertex_shader_apply;
    }
    return shader;
}

Vertex flat_shading_vertex_shader_apply(VertexShader* shader, const Vertex* in) {
    // calculate position based on rotation and translation
    Vec3 multiplied;
    multiply_matrix_by_point(shader->rotation, &in->pos, &multiplied);
    
    // calculate intensity based on angle of incidence
    Vec3 temp;
    multiply_matrix_by_point(shader->rotation, &in->n, &temp);
    Vec3 d = vec3_multiply(&diffuse, fmax(0.0f, -dot_product(&temp, &dir)));

    // add diffuse+ambient
    temp = vec3_add(&d, &ambient);

    // filter by material color
    temp = vec3_hadamard(&color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    Vertex vertexOut;
    vertexOut.pos = vec3_add(&multiplied, &shader->translation);
    vertexOut.tc = in->tc;
    vertexOut.col = temp;

    return vertexOut;
}
        
      