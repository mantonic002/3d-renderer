#include "vertex_shader.h"

VertexShader* create_point_light_vertex_shader() {
    VertexShader* shader = (VertexShader*)malloc(sizeof(VertexShader));
    if (shader) {
        shader->apply = point_light_vertex_shader_apply;
        shader->light_diffuse = (Vec3){ 0.5f,0.5f,0.5f };
        shader->light_ambient = (Vec3){ 0.1f,0.1f,0.1f };
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
    Vec4 pos_v4 = vec4(&in->pos.as_vec3, 1.0f);
    Vec4 pos_v4_proj = multiply_matrix_by_vec4(shader->world_view_proj, &pos_v4);

    Vec3 pos = vec3(&pos_v4_proj);

    // light to vertex distance and direction
    const Vec3 v_to_l = vec3_subtract(&shader->light_pos, &pos);
    const float dist = vec3_len(&v_to_l);
    const Vec3 dir = vec3_divide(&v_to_l, dist);

    // attenuation
    const float attenuation = 1.0f / (constant_attenuation + linear_attenuation * dist + quadradic_attenuation * SQUARE(dist));

    // intensity based on angle of incidence and attenuation
    Vec3 temp = multiply_matrix_by_vec3(shader->world, &in->n);
    Vec3 diffuse_x_attenuation = vec3_multiply(&shader->light_diffuse, attenuation);
    Vec3 d = vec3_multiply(&diffuse_x_attenuation, fmax(0.0f, dot_product(&temp, &dir)));

    // add diffuse+ambient
    temp = vec3_add(&d, &shader->light_ambient);

    // filter by material color
    temp = vec3_hadamard(&shader->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    Vertex vertexOut;
    vertexOut.pos.as_vec4 = pos_v4_proj;
    vertexOut.tc = in->tc;
    vertexOut.n = in->n;
    vertexOut.col = temp;

    return vertexOut;
}
        
      