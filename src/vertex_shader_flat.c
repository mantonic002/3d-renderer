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
    Vec4 pos_v4 = vec4(&in->pos.as_vec3, 1.0f);
    Vec4 pos_v4_proj = multiply_matrix_by_vec4(shader->worldProj, &pos_v4);

    // calculate intensity based on angle of incidence
    Vec3 temp = multiply_matrix_by_vec3(shader->world, &in->n);
    Vec3 d = vec3_multiply(&shader->light_diffuse, fmax(0.0f, -dot_product(&temp, &shader->light_pos)));

    // add diffuse+ambient
    temp = vec3_add(&d, &shader->light_ambient);

    // filter by material color
    temp = vec3_hadamard(&shader->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    Vertex vertexOut;
    vertexOut.pos.as_vec4 = pos_v4_proj;
    vertexOut.tc = in->tc;
    vertexOut.n = multiply_matrix_by_vec3(shader->world, &in->n);
    vertexOut.col = temp;

    Vec4 world_pos = multiply_matrix_by_vec4(shader->world, &pos_v4);
    vertexOut.world_pos = vec3(&world_pos);

    return vertexOut;
}
        
      