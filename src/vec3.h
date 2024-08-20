#ifndef VEC3_H
#define VEC3_H

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

Vec3 cross_product (const Vec3* v1, const Vec3* v2);
float dot_product (const Vec3* v1, const Vec3* v2);
Vec3 vec3_subtract (const Vec3* v1, const Vec3* v2);
Vec3 vec3_interpolate (const Vec3* v1, const Vec3* v2, const float alpha);


#endif