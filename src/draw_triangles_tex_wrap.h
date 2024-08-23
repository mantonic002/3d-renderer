#ifndef DRAW_TRIANGLES_TEX_H
#define DRAW_TRIANGLES_TEX_H

#include "vertex.h"
#include "helper.h"

#include <SDL2/SDL.h>

void draw_triangle_tex_wrap (Vertex* v1, Vertex* v2, Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_top_triangle_tex_wrap (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_bottom_triangle_tex_wrap (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_triangle_tex_wrap (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer,
                             const Vertex* dv1, const Vertex* dv2, Vertex* itEdge2);

#endif