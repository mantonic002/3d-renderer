#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "draw_triangles.h"
#include "draw_triangles_tex.h"

#include <SDL2/SDL.h>

void render(SDL_Renderer** renderer, Scene* scene);

#endif