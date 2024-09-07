#include "pixel_shader.h"

SDL_Color shader_effect_per_pixel_light_specular(PixelShader* ps, Vertex* vertex) {
    // values for calculating lighting
    float linear_attenuation = 1.0f;
    float quadradic_attenuation = 2.619f;
    float constant_attenuation = 0.382f;

    //  normalize interpolated normal
    Vec3 surface_norm = vec3_normalize(&vertex->n);
    surface_norm= vec3_multiply(&surface_norm, -1.0f);

    // light to vertex distance and direction
    const Vec3 v_to_l = vec3_subtract(&ps->light_pos, &vertex->world_pos);
    const float dist = vec3_len(&v_to_l);
    const Vec3 dir = vec3_divide(&v_to_l, dist);

    // attenuation
    const float attenuation = 1.0f / (constant_attenuation + linear_attenuation * dist + quadradic_attenuation * SQUARE(dist));

    // intensity based on angle of incidence and attenuation
    Vec3 diffuse_x_attenuation = vec3_multiply(&ps->light_diffuse, attenuation);
    Vec3 diffuse = vec3_multiply(&diffuse_x_attenuation, fmax(0.0f, dot_product(&surface_norm, &dir)));

    // reflected light vector
    Vec3 temp = vec3_multiply(&surface_norm, dot_product(&v_to_l, &surface_norm) * 2.0f);
    Vec3 reflected = vec3_subtract(&temp, &v_to_l);

    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    Vec3 neg_refl = vec3_multiply(&reflected, -1);
    neg_refl = vec3_normalize(&neg_refl);
    Vec3 wrld_pos_norm = vec3_normalize(&vertex->world_pos);

    temp = vec3_multiply(&ps->light_diffuse, ps->specular_intensity);

    Vec3 spec =vec3_multiply(&temp, pow(fmax(0.0f, dot_product(&neg_refl, &wrld_pos_norm)), ps->specular_power));


    // add diffuse+ambient+specular
    temp = vec3_add(&diffuse, &ps->light_ambient);
    temp = vec3_add(&temp, &spec);

    // filter by material color
    temp = vec3_hadamard(&ps->light_color, &temp);

    // saturate and scale
    temp = vec3_saturate(&temp);

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
    } else {
        c = (SDL_Color){
            temp.x * 255.0f,
            temp.y * 255.0f,
            temp.z * 255.0f,
        };
    }

    return c;
}

PixelShader* create_pixel_shader_per_pixel_lighting_specular(const char* filename, float specular_power) {
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
        ps->specular_power = specular_power;
        ps->specular_intensity = 1.0f;
        ps->shade = shader_effect_per_pixel_light_specular;
    }
    return ps;
}
