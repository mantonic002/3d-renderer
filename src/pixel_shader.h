#ifndef SHADER_H
#define SHADER_H

#include "vertex.h"
#include "helper.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct PixelShader {
    SDL_Surface* texture;
    float tex_width;
    float tex_height; 
    float tex_clamp_x;
    float tex_clamp_y;
    SDL_Color (*shade)(struct PixelShader* ps, Vertex* Vertex);
} PixelShader;

void pixel_shader_bind_texture(PixelShader* p, const char* filename);
PixelShader* create_pixel_shader_texture(const char* filename);

#endif