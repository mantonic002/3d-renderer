#ifndef SCENE_H
#define SCENE_H

#include "vec2.h"
#include "vec3.h"
#include "vertex.h"
#include "constants.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Scene {
    Vertex* vertices;
    Vec3* indices;
    float angle_x;
    float angle_y;
    float angle_z;
    float z_offset;
    SDL_Surface* texture;
} Scene;

Scene make_cube(SDL_Surface** texture, float texture_dimension);
void restart_cube(Scene* scene);

#endif