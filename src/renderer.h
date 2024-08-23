#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "pipeline.h"
#include <SDL2/SDL.h>

void render(SDL_Renderer** renderer, Scene* scene);

#endif