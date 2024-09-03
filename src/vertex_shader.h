#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "vertex.h"
#include "helper.h"
#include "constants.h"

typedef struct VertexShader{
    float rotation[3][3]; //3x3 rotation matrix
    Vec3 translation;
    float time; // for wave effect
    Vec3 light_pos;
    Vec3 light_diffuse;
    Vec3 light_ambient;
    Vec3 light_color;
    Vertex (*apply)(struct VertexShader* shader, const Vertex* in);
} VertexShader;

VertexShader* create_default_vertex_shader();
Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in);

VertexShader* create_wave_vertex_shader(const float rotation[3][3], const Vec3* translation, float time);
Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in);

VertexShader* create_flat_shading_vertex_shader();
Vertex flat_shading_vertex_shader_apply(VertexShader* shader, const Vertex* in);
        
VertexShader* create_point_light_vertex_shader();
Vertex point_light_vertex_shader_apply(VertexShader* shader, const Vertex* in);

#endif