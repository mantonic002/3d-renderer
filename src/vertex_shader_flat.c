#include "vertex_shader.h"

VertexShader* create_flat_shading_vertex_shader() {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->apply = flat_shading_vertex_shader_apply;
        shader->light_pos =     (Vec3){ 1.0f,1.0f,0.0f };
        shader->light_diffuse = (Vec3){ 1.0f,1.0f,1.0f };
        shader->light_ambient = (Vec3){ 0.3f,0.3f,0.3f };
        shader->light_color =   (Vec3){ 0.8f,0.85f,1.0f };
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
    Vec3 d = vec3_multiply(&shader->light_diffuse, fmax(0.0f, -dot_product(&temp, &shader->light_pos)));

    // add diffuse+ambient
    temp = vec3_add(&d, &shader->light_ambient);

    // filter by material color
    temp = vec3_hadamard(&shader->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    Vertex vertexOut;
    vertexOut.pos = vec3_add(&multiplied, &shader->translation);
    vertexOut.tc = in->tc;
    vertexOut.col = temp;

    return vertexOut;
}
        
      