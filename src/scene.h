#ifndef SCENE_H
#define SCENE_H

#include "vec2.h"
#include "vec3.h"
#include "tex_vertex.h"
#include "constants.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Scene {
    bool wireframe;
    bool textured;
    Vec3* vertices;
    TexVertex* projected_points;
    Vec2* edges;
    Vec3* indices;
    bool* cullFlags;
    float angle_x;
    float angle_y;
    float angle_z;
    float z_offset;
    SDL_Surface* texture;
} Scene;

Scene make_cube(SDL_Surface** texture, float texture_dimension);
void restart_cube(Scene* scene);

#endif