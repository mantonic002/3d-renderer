#include "geometry_shader.h"

Triangle default_geometry_shader(struct GeometryShader* gs, const Vertex* in1, const Vertex* in2, const Vertex* in3, int triangle_index) {
    Triangle ret = { *in1, *in2, *in3 };
    return ret;
}

GeometryShader* create_default_geometry_shader() {
    GeometryShader* shader = (GeometryShader*)malloc(sizeof(GeometryShader));
    if (shader) {
        shader->shade = default_geometry_shader;
    }
    return shader;
}