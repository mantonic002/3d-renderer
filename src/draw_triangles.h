#ifndef DRAW_TRIANGLES_H
#define DRAW_TRIANGLES_H

#include "vec2.h"
#include "helper.h"

#include <SDL2/SDL.h>


void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer);
void draw_flat_top_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer);
void draw_flat_bottom_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer);

#endif