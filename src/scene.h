#ifndef SCENE_H
#define SCENE_H

#include "vec2.h"
#include "vec3.h"
#include "mat.h"
#include "vertex.h"
#include "constants.h"
#include "pipeline.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Scene {
    IndexedTriangleList* triList;
    IndexedTriangleList* light_sphere;
    Pipeline* pipeline;
    Pipeline* light_pipeline;
    // fov
    float aspect_ratio;
    float hfov;
    float vfov;
    // variables for camera
    float htrack;
    float vtrack;
    float cam_speed;
    Vec3 cam_pos;
    Mat cam_rot;
    // variables for model
    Vec3 mod_pos;
    float angle_x;
    float angle_y;
    float angle_z;
    // variables for light
    Vec3 lpos;

    float time;
    void (*draw)(struct Scene* scene, SDL_Renderer** renderer);

} Scene;

Scene make_scene(SDL_Renderer** renderer, const char* filename);

void cube_init_triangle_list(Scene* scene, float texture_dimension);
void cube_init_triangle_list_skinned(Scene* scene);

void plane_init_triangle_list(Scene* scene, int divisions, float size);
void plane_init_triangle_list_skinned(Scene* scene, int divisions, float size);
Scene make_scene_plane(SDL_Renderer** renderer, const char* filename);
void scene_plane_draw(Scene* scene, SDL_Renderer** renderer);

Scene make_scene_flat_shade(SDL_Renderer** renderer);
void scene_flat_shade_draw(Scene* scene, SDL_Renderer** renderer);
void cube_init_triangle_list_independent_faces_w_normals(Scene* scene);

void scene_point_light_per_vertex_draw(Scene* scene, SDL_Renderer** renderer);
Scene make_scene_point_light_per_vertex(SDL_Renderer** renderer, const char* filename);

void scene_point_light_per_pixel_draw(Scene* scene, SDL_Renderer** renderer);
Scene make_scene_point_light_per_pixel(SDL_Renderer** renderer, const char* filename);

IndexedTriangleList* sphere_init_triangle_list(float radius, int latDiv, int longDiv);
void sphere_init_normals(Scene* scene, float radius, int latDiv, int longDiv);

void scene_point_light_per_pixel_specular_draw(Scene* scene, SDL_Renderer** renderer);
Scene make_scene_point_light_per_pixel_specular(SDL_Renderer** renderer, const char* filename, float specular_power);

Vec2 convert_tex_coord(float u, float v);

#endif