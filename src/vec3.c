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

float vec3_len(const Vec3* v) {
    float length = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    return length;
}

Vec3 vec3_normalize(const Vec3* v) {
    Vec3 normalized;
    float length = vec3_len(v);

    // Prevent division by zero
    if (length > 0.0f) {
        normalized.x = v->x / length;
        normalized.y = v->y / length;
        normalized.z = v->z / length;
    } else {
        // If the length is zero, return a zero vector (or handle it appropriately)
        normalized.x = 0.0f;
        normalized.y = 0.0f;
        normalized.z = 0.0f;
    }

    return normalized;
}

