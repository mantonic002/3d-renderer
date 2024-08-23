#ifndef PIPELINE_H
#define PIPELINE_H

#include "indexed_triangle_list.h"
#include "constants.h"
#include "triangle.h"
#include "helper.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Pipeline {
    float rotation[3][3];  //rotation is 2d array representing a 3x3 matrix
    Vec3 translation;
    SDL_Surface* texture;
    SDL_Renderer** renderer;
} Pipeline;

void pipeline_draw(Pipeline* p, IndexedTriangleList* triList);
void pipeline_bind_rotation(Pipeline* p, const float rotationIn[3][3]); //rotationIn is 3x3 array representing a 3x3 matrix
void pipeline_bind_texture(Pipeline* p, const char* filename);

void process_vertices (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI);
void assemble_triangles (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI);
void process_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3);
void post_process_triangle (Pipeline* p, Triangle* triangle);
void draw_triangle (Pipeline* p, Triangle* triangle);
void draw_flat_top_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3);
void draw_flat_bottom_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3);
void draw_flat_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3, const Vertex* dv1, const Vertex* dv2, Vertex* itEdge2);

void transform(Vec3* v);

#endif