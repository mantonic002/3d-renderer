#ifndef MAT_H
#define MAT_H

#include "vec3.h"
#include "vec4.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Mat{
    int size;
    float** data;
} Mat;

Mat create_matrix(int size);
Mat mat_identity(int size);
Mat mat_scaling(float factor, int size);
Mat mat_rotation_x(float angle, int size);
Mat mat_rotation_y (float angle, int size);
Mat mat_rotation_z (float angle, int size);
Mat mat_translation(float x, float y, float z);
Mat mat_projection(float w, float h, float n, float f);
Mat mat_projection_hfov(float fov, float ar, float n, float f);
Vec3 multiply_matrix_by_vec3(const Mat matrix, const Vec3* point);
Vec4 multiply_matrix_by_vec4(const Mat matrix, const Vec4* point);
Mat multiply_matrices(const Mat matrixA, const Mat matrixB);
Mat mat_transposition(const Mat mat);

#endif