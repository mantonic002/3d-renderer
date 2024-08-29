#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H

#include "vertex.h"
#include "triangle.h"
#include "helper.h"

typedef struct GeometryShader {
    Triangle (*shade)(struct GeometryShader* gs, const Vertex* in1, const Vertex* in2, const Vertex* in3, int triangle_index);
} GeometryShader;

Triangle default_geometry_shader(struct GeometryShader* gs, const Vertex* in1, const Vertex* in2, const Vertex* in3, int triangle_index);
GeometryShader* create_default_geometry_shader(void);

#endif