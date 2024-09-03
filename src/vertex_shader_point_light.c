#include "vertex_shader.h"

VertexShader* create_point_light_vertex_shader() {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->apply = flat_shading_vertex_shader_apply;
        shader->light_diffuse = (Vec3){ 1.0f,1.0f,1.0f };
        shader->light_ambient = (Vec3){ 0.3f,0.3f,0.3f };
        shader->light_color =   (Vec3){ 0.8f,0.85f,1.0f };
    }
    return shader;
}

Vertex point_light_vertex_shader_apply(VertexShader* shader, const Vertex* in) {
    // values for calculating lighting
    float linear_attenuation = 1.0f;
    float quadradic_attenuation = 2.619f;
    float constant_attenuation = 0.382f;

    // calculate position based on rotation and translation
    Vec3 multiplied;
    multiply_matrix_by_point(shader->rotation, &in->pos, &multiplied);
    Vec3 pos = vec3_add(&multiplied, &shader->translation);

    // light to vertex distance and direction
    const Vec3 v_to_l = vec3_subtract(&shader->light_pos, &pos);
    const float dist = vec3_len(&v_to_l);
    const Vec3 dir = vec3_divide(&v_to_l, dist);

    // attenuation
    const float attenuation = 1.0f / (constant_attenuation + linear_attenuation * dist * quadradic_attenuation * SQUARE(dist));

    // intensity based on angle of incidence and attenuation
    Vec3 temp;
    multiply_matrix_by_point(shader->rotation, &in->n, &temp);
    Vec3 diffuse_x_attenuation = vec3_multiply(&shader->light_diffuse, attenuation);
    Vec3 d = vec3_multiply(&diffuse_x_attenuation, fmax(0.0f, dot_product(&temp, &dir)));

    // add diffuse+ambient
    temp = vec3_add(&d, &shader->light_ambient);

    // filter by material color
    temp = vec3_hadamard(&shader->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    Vertex vertexOut;
    vertexOut.pos = pos;
    vertexOut.tc = in->tc;
    vertexOut.col = temp;

    return vertexOut;
}
        
      