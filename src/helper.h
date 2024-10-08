#ifndef HELPER_H
#define HELPER_H

#include "vec2.h"
#include "vec3.h"

#include <SDL2/SDL.h>

void ptr_swap(void* p1, void* p2, size_t size);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
SDL_Color get_pixel_rgba(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, SDL_Color* c);
void clear_surface(SDL_Surface* surface);

#endif