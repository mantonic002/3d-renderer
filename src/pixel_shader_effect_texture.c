#include "pixel_shader.h"

SDL_Color shader_effect_texture(PixelShader* ps, Vertex* vertex) {
    SDL_Color c;
    //texture lookup, clamp
    get_pixel_rgba( ps->texture, 
                    fmin(vertex->tc.x * ps->tex_width, ps->tex_clamp_x),
                    fmin(vertex->tc.y * ps->tex_height, ps->tex_clamp_y),
                    &c.r, &c.g, &c.b, &c.a);

    return c;
}

void pixel_shader_bind_texture(PixelShader* p, const char* filename) {
    SDL_Surface* texture = IMG_Load(filename);
    if (!texture) {
        fprintf(stderr, "Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    p->texture = texture;

    p->tex_width = (float)texture->w;
    p->tex_height = (float)texture->h; 
    p->tex_clamp_x = p->tex_width - 1.0f;
    p->tex_clamp_y = p->tex_height - 1.0f;
}

PixelShader* create_pixel_shader_texture(const char* filename) {
    PixelShader* ps = (PixelShader*)malloc(sizeof(PixelShader));
    if (ps) {
        pixel_shader_bind_texture(ps, filename);
        ps->shade = shader_effect_texture;
    }
    return ps;
}