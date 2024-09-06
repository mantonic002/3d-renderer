#ifndef VEC4_H
#define VEC4_H

#include "vec3.h"
#include <math.h>

typedef struct Vec4
{
    float x;
    float y;
    float z;
    float w;
} Vec4;

// Function prototypes
Vec4 vec4(const Vec3* v, int w);
Vec3 vec3(Vec4* v);
Vec4 vec4_add(const Vec4* v1, const Vec4* v2);
Vec4 vec4_subtract(const Vec4* v1, const Vec4* v2);
Vec4 vec4_multiply(const Vec4* v, float scalar);
Vec4 vec4_divide(const Vec4* v, float scalar);
Vec4 vec4_interpolate (const Vec4* v1, const Vec4* v2, const float alpha);
Vec4 vec4_hadamard(const Vec4* v1, const Vec4* v2);
Vec4 vec4_saturate(const Vec4* v);
float vec4_len(const Vec4* v);
Vec4 vec4_normalize(const Vec4* v);
float vec4_dot(const Vec4* v1, const Vec4* v2);

#endif // VEC4_H
