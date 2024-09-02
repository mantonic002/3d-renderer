#include "mat3.h"

Mat3 mat3_rotation_x(float angle) {
    Mat3 ret;
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    ret.data[0][0] = 1;
    ret.data[0][1] = 0;
    ret.data[0][2] = 0;
    ret.data[1][0] = 0;
    ret.data[1][1] = cos_angle;
    ret.data[1][2] = -sin_angle;
    ret.data[2][0] = 0;
    ret.data[2][1] = sin_angle;
    ret.data[2][2] = cos_angle;

    return ret;
}

Mat3 mat3_rotation_y(float angle) {
    Mat3 ret;
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    ret.data[0][0] = cos_angle;
    ret.data[0][1] = 0;
    ret.data[0][2] = sin_angle;
    ret.data[1][0] = 0;
    ret.data[1][1] = 1;
    ret.data[1][2] = 0;
    ret.data[2][0] = -sin_angle;
    ret.data[2][1] = 0;
    ret.data[2][2] = cos_angle;

    return ret;
}

Mat3 mat3_rotation_z(float angle) {
    Mat3 ret;
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    ret.data[0][0] = cos_angle;
    ret.data[0][1] = -sin_angle;
    ret.data[0][2] = 0;
    ret.data[1][0] = sin_angle;
    ret.data[1][1] = cos_angle;
    ret.data[1][2] = 0;
    ret.data[2][0] = 0;
    ret.data[2][1] = 0;
    ret.data[2][2] = 1;

    return ret;
}

Mat3 multiply_matrices(const Mat3 matrixA, const Mat3 matrixB) {
    Mat3 result;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.data[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                result.data[i][j] += matrixA.data[i][k] * matrixB.data[k][j];
            }
        }
    }

    return result;
}
