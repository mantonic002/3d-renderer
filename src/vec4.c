#include "vec4.h"

Vec4 vec4(const Vec3* v, int w) {
    Vec4 result = {
        v->x,
        v->y,
        v->y,
        w
    };
    return result;
}

Vec3 vec3(Vec4* v) {
    Vec3 result = {
        v->x,
        v->y,
        v->y
    };
    return result;
}

Vec4 vec4_add(const Vec4* v1, const Vec4* v2) {
    Vec4 result = {
        v1->x + v2->x,
        v1->y + v2->y,
        v1->z + v2->z,
        v1->w + v2->w
    };
    return result;
}

Vec4 vec4_subtract(const Vec4* v1, const Vec4* v2) {
    Vec4 result = {
        v1->x - v2->x,
        v1->y - v2->y,
        v1->z - v2->z,
        v1->w - v2->w
    };
    return result;
}

Vec4 vec4_multiply(const Vec4* v, float scalar) {
    Vec4 result = {
        v->x * scalar,
        v->y * scalar,
        v->z * scalar,
        v->w * scalar
    };
    return result;
}

Vec4 vec4_divide(const Vec4* v, float scalar) {
    Vec4 result = {
        v->x / scalar,
        v->y / scalar,
        v->z / scalar,
        v->w / scalar
    };
    return result;
}

Vec4 vec4_interpolate (const Vec4* v1, const Vec4* v2, const float alpha) {
    Vec4 result = {
        v1->x + (v2->x - v1->x) * alpha,
        v1->y + (v2->y - v1->y) * alpha,
        v1->z + (v2->z - v1->z) * alpha,
        v1->w + (v2->w - v1->w) * alpha,
    };
    return result;
}

Vec4 vec4_hadamard(const Vec4* v1, const Vec4* v2) {
    Vec4 result = {
        v1->x * v2->x,
        v1->y * v2->y,
        v1->z * v2->z,
        v1->w * v2->w
    };
    return result;
}

Vec4 vec4_saturate(const Vec4* v) {
    Vec4 result = {
        fminf(1.0f, fmaxf(0.0f, v->x)),
        fminf(1.0f, fmaxf(0.0f, v->y)),
        fminf(1.0f, fmaxf(0.0f, v->z)),
        fminf(1.0f, fmaxf(0.0f, v->w))
    };
    return result;
}

float vec4_len(const Vec4* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vec4 vec4_normalize(const Vec4* v) {
    Vec4 normalized;
    float length = vec4_len(v);

    // Prevent division by zero
    if (length > 0.0f) {
        normalized.x = v->x / length;
        normalized.y = v->y / length;
        normalized.z = v->z / length;
    } else {
        normalized.x = 0.0f;
        normalized.y = 0.0f;
        normalized.z = 0.0f;
    }
    normalized.w = v->w;

    return normalized;
}

float vec4_dot(const Vec4* v1, const Vec4* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}
