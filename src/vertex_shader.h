#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "vertex.h"
#include "mat.h"
#include "helper.h"
#include "constants.h"

typedef struct VertexShader{
    Mat world;
    Mat proj;
    Mat worldProj;
    float time; // for wave effect
    Vec3 light_pos;
    Vec3 light_diffuse;
    Vec3 light_ambient;
    Vec3 light_color;
    Vertex (*apply)(struct VertexShader* shader, const Vertex* in);
} VertexShader;

void bind_world(VertexShader* shader, Mat transformation);
void bind_projection(VertexShader* shader, Mat transformation);

VertexShader* create_default_vertex_shader();
Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in);

// VertexShader* create_wave_vertex_shader(const Mat rotation, const Vec3* translation, float time);
// Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in);

// VertexShader* create_flat_shading_vertex_shader();
// Vertex flat_shading_vertex_shader_apply(VertexShader* shader, const Vertex* in);
        
// VertexShader* create_point_light_vertex_shader();
// Vertex point_light_vertex_shader_apply(VertexShader* shader, const Vertex* in);

#endif