#include "pixel_shader.h"

SDL_Color shader_effect_flat_shade(PixelShader* ps, Vertex* vertex) {
    SDL_Color c = {
        vertex->col.x * 255.0f,
        vertex->col.y * 255.0f,
        vertex->col.z * 255.0f,
    };

    return c;
}

PixelShader* create_pixel_shader_flat_shade() {
    PixelShader* ps = (PixelShader*)malloc(sizeof(PixelShader));
    if (ps) {
        ps->shade = shader_effect_flat_shade;
    }
    return ps;
}