#ifndef HELPER_H
#define HELPER_H

#include "vec2.h"
#include "vec3.h"

#include <SDL2/SDL.h>

void ptr_swap(void* p1, void* p2, size_t size);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void get_pixel_rgba(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
void multiplyMatrixByPoint(const float matrix[3][3], const Vec3* point, Vec3* result);
void multiply_matrices(const float matrixA[3][3], const float matrixB[3][3], float result[3][3]);

#endif