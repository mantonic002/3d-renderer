#include "vec3.h"


Vec3 cross_product (const Vec3* v1, const Vec3* v2) {
    Vec3 result = { 
        (v1->y * v2->z) - (v1->z * v2->y), 
        (v1->z * v2->x) - (v1->x * v2->z), 
        (v1->x * v2->y) - (v1->y * v2->x), 
    };
    return result;
}

float dot_product (const Vec3* v1, const Vec3* v2) {
    float result = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z; 
    return result;
}

Vec3 vec3_subtract (const Vec3* v1, const Vec3* v2) {
    Vec3 result = { 
        v1->x - v2->x,
        v1->y - v2->y,
        v1->z - v2->z,
    };
    return result;
}


Vec3 vec3_interpolate (const Vec3* v1, const Vec3* v2, const float alpha) {
    Vec3 result = {
        v1->x + (v2->x - v1->x) * alpha,
        v1->y + (v2->y - v1->y) * alpha,
        v1->z + (v2->z - v1->z) * alpha,
    };
    return result;
}

Vec3 vec3_add (const Vec3* v1, const Vec3* v2) {
    Vec3 result = { 
        v1->x + v2->x,
        v1->y + v2->y,
        v1->z + v2->z,
    };
    return result;
}

Vec3 vec3_divide (const Vec3* v, float scalar) {
    Vec3 result = { 
        v->x/scalar,
        v->y/scalar,
        v->z/scalar,
    };
    return result;
}

Vec3 vec3_multiply (const Vec3* v, float scalar) {
    Vec3 result = { 
        v->x*scalar,
        v->y*scalar,
        v->z*scalar,
    };
    return result;
}

Vec3 vec3_hadamard (const Vec3* v1, const Vec3* v2) {
    Vec3 result = {
        v1->x * v2->x,
        v1->y * v2->y,
        v1->z * v2->z,
    };
    return result;
}

Vec3 vec3_saturate (const Vec3* v) {
    Vec3 result = {
        fmin(1.0f, fmax(0.0f, v->x)),
        fmin(1.0f, fmax(0.0f, v->y)),
        fmin(1.0f, fmax(0.0f, v->z)),
    };
    return result;
}

