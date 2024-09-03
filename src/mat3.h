#ifndef MAT3_H
#define MAT3_H

#include <math.h>

typedef struct Mat3{
    float data[3][3];
} Mat3;

Mat3 mat3_rotation_x (float angle);
Mat3 mat3_rotation_y (float angle);
Mat3 mat3_rotation_z (float angle);
Mat3 multiply_matrices(const Mat3 matrixA, const Mat3 matrixB);

#endif