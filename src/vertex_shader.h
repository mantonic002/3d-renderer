#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "vertex.h"
#include "helper.h"

typedef struct VertexShader{
    float rotation[3][3]; //3x3 rotation matrix
    Vec3 translation;
    Vertex (*apply)(struct VertexShader* shader, const Vertex* in);
} VertexShader;

VertexShader* create_default_vertex_shader(const float rotation[3][3], const Vec3* translation);

Vertex default_vertex_shader_apply(VertexShader* shader, const Vertex* in);

#endif