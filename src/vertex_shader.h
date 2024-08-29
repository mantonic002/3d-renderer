#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "vertex.h"
#include "helper.h"

// constants for wave effect
#define freqWave 10.0f
#define freqScroll 5.0f
#define amplitude 0.05f

typedef struct VertexShader{
    float rotation[3][3]; //3x3 rotation matrix
    Vec3 translation;
    float time;
    Vertex (*apply)(struct VertexShader* shader, const Vertex* in);
} VertexShader;

VertexShader* create_default_vertex_shader(const float rotation[3][3], const Vec3* translation);
Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in);

VertexShader* create_wave_vertex_shader(const float rotation[3][3], const Vec3* translation, float time);
Vertex wave_vertex_shader_apply(VertexShader* shader, const Vertex* in);
        
#endif