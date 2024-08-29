#ifndef PIPELINE_H
#define PIPELINE_H

#include "indexed_triangle_list.h"
#include "constants.h"
#include "triangle.h"
#include "pixel_shader.h"
#include "vertex_shader.h"
#include "geometry_shader.h"
#include "helper.h"
#include "z_buffer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Pipeline {
    VertexShader* vertex_shader;
    GeometryShader* geometry_shader;
    PixelShader* pixel_shader;
    SDL_Renderer** renderer;
    ZBuffer* zb;
} Pipeline;

void pipeline_draw(Pipeline* p, IndexedTriangleList* triList);
void pipeline_begin_frame(Pipeline* p);

void process_vertices (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI);
void assemble_triangles (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI);
void process_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3, int triangle_index);
void post_process_triangle (Pipeline* p, Triangle* triangle);
void draw_triangle (Pipeline* p, Triangle* triangle);
void draw_flat_top_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3);
void draw_flat_bottom_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3);
void draw_flat_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3, const Vertex* dv1, const Vertex* dv2, Vertex* itEdge2);

void transform(Vertex* v);

#endif