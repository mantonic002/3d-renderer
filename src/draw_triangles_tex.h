#ifndef DRAW_TRIANGLES_TEX_H
#define DRAW_TRIANGLES_TEX_H

#include "tex_vertex.h"
#include "helper.h"
#include <SDL2/SDL.h>

void draw_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_top_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);
void draw_flat_bottom_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer);

#endif