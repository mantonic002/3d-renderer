#ifndef SCENE_H
#define SCENE_H

#include "vec2.h"
#include "vec3.h"
#include "mat3.h"
#include "vertex.h"
#include "constants.h"
#include "pipeline.h"
#include "obj_loader.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Scene {
    IndexedTriangleList* triList;
    IndexedTriangleList* light_sphere;
    float angle_x;
    float angle_y;
    float angle_z;
    float z_offset;
    Vec3 lpos;
    float time;
    Pipeline* pipeline;
    Pipeline* light_pipeline;
    void (*draw)(struct Scene* scene, SDL_Renderer** renderer);

} Scene;

Scene make_scene(SDL_Renderer** renderer, const char* filename);

void scene_cube_draw(Scene* scene, SDL_Renderer** renderer);
void scene_double_cube_draw(Scene* scene, SDL_Renderer** renderer);

void cube_init_triangle_list(Scene* scene, float texture_dimension);
void cube_init_triangle_list_skinned(Scene* scene);

void plane_init_triangle_list(Scene* scene, int divisions, float size);
void plane_init_triangle_list_skinned(Scene* scene, int divisions, float size);
void scene_plane_draw(Scene* scene, SDL_Renderer** renderer);

Scene make_scene_flat_shade(SDL_Renderer** renderer);
void scene_flat_shade_draw(Scene* scene, SDL_Renderer** renderer);
void cube_init_triangle_list_independent_faces_w_normals(Scene* scene);

void model_init_triangle_list(Scene* scene, const char *filename);
void scene_model_draw(Scene* scene, SDL_Renderer** renderer);
Scene make_scene_model(SDL_Renderer** renderer, const char* filename);

IndexedTriangleList* sphere_init_triangle_list(float radius, int latDiv, int longDiv);
void sphere_init_normals(Scene* scene, float radius, int latDiv, int longDiv);

Vec2 convert_tex_coord(float u, float v);

#endif