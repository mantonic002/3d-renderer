#ifndef DRAW_TRIANGLES_TEX_H
#define DRAW_TRIANGLES_TEX_H

#include "tex_vertex.h"
#include "helper.h"

#include <SDL2/SDL.h>

void draw_triangle_tex_wrap (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_top_triangle_tex_wrap (const TexVertex* v1, const TexVertex* v2, const TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_bottom_triangle_tex_wrap (const TexVertex* v1, const TexVertex* v2, const TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_triangle_tex_wrap (const TexVertex* v1, const TexVertex* v2, const TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer,
                             const TexVertex* dv1, const TexVertex* dv2, TexVertex* itEdge2);

#endif