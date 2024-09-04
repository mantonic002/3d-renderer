#include "pixel_shader.h"

SDL_Color shader_effect_flat_tex(PixelShader* ps, Vertex* vertex) {
    //texture lookup, clamp
    SDL_Color c = get_pixel_rgba( ps->texture, 
                    fmin(vertex->tc.x * ps->tex_width, ps->tex_clamp_x),
                    fmin(vertex->tc.y * ps->tex_height, ps->tex_clamp_y));
    
    SDL_Color shade = {
        vertex->col.x * c.r,
        vertex->col.y * c.g,
        vertex->col.z * c.b,
    };

    return shade;
}

PixelShader* create_pixel_shader_flat_tex(const char* filename) {
    PixelShader* ps = (PixelShader*)malloc(sizeof(PixelShader));
    if (ps) {
        pixel_shader_bind_texture(ps, filename);
        ps->shade = shader_effect_flat_tex;
    }
    return ps;
}
