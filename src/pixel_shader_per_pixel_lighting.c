#include "pixel_shader.h"

float x = 0;
SDL_Color shader_effect_per_pixel_light(PixelShader* ps, Vertex* vertex) {
    // values for calculating lighting
    float linear_attenuation = 1.0f;
    float quadradic_attenuation = 2.619f;
    float constant_attenuation = 0.382f;

    // light to vertex distance and direction
    const Vec3 v_to_l = vec3_subtract(&ps->light_pos, &vertex->world_pos);
    const float dist = vec3_len(&v_to_l);
    const Vec3 dir = vec3_divide(&v_to_l, dist);

    // attenuation
    const float attenuation = 1.0f / (constant_attenuation + linear_attenuation * dist + quadradic_attenuation * SQUARE(dist));

    // intensity based on angle of incidence and attenuation
    Vec3 diffuse_x_attenuation = vec3_multiply(&ps->light_diffuse, attenuation);

    //normalize interpolated normal
    Vec3 norm = vec3_normalize(&vertex->n);
    Vec3 d = vec3_multiply(&diffuse_x_attenuation, fmax(0.0f, dot_product(&norm, &dir)));

    // add diffuse+ambient
    Vec3 temp = vec3_add(&d, &ps->light_ambient);

    // filter by material color
    temp = vec3_hadamard(&ps->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

    temp = vec3_multiply(&temp, 255.0f);

    SDL_Color c;
    if (ps->texture) {
        //texture lookup, clamp
        SDL_Color texCol = get_pixel_rgba( ps->texture, 
                        fmin(vertex->tc.x * ps->tex_width, ps->tex_clamp_x),
                        fmin(vertex->tc.y * ps->tex_height, ps->tex_clamp_y));
        c = (SDL_Color){
            temp.x * texCol.r,
            temp.y * texCol.g,
            temp.z * texCol.b,
        };
    }
        c = (SDL_Color){
            temp.x,
            temp.y,
            temp.z,
        };
    return c;
}

PixelShader* create_pixel_shader_per_pixel_lighting(const char* filename) {
    PixelShader* ps = (PixelShader*)malloc(sizeof(PixelShader));
    if (ps) {
        if (filename[0] != '\0') {
            pixel_shader_bind_texture(ps, filename);
        } else {
            ps->texture = NULL;
        }
        ps->light_diffuse = (Vec3){ 0.5f,0.5f,0.5f };
        ps->light_ambient = (Vec3){ 0.1f,0.1f,0.1f };
        ps->light_color =   (Vec3){ 0.8f,0.85f,1.0f };
        ps->shade = shader_effect_per_pixel_light;
    }
    return ps;
}
