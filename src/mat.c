#include "mat.h"

Mat create_matrix(int size) {
    Mat mat;
    mat.size = size;
    mat.data = malloc(size * sizeof(float*));

    for (int i = 0; i < size; ++i) {
        mat.data[i] = malloc(size * sizeof(float));
    }

    return mat;
}

Mat mat_identity(int size) {
    Mat ret = create_matrix(size);
    
    ret.data[0][0] = 1;
    ret.data[0][1] = 0;
    ret.data[0][2] = 0;
    ret.data[1][0] = 0;
    ret.data[1][1] = 1;
    ret.data[1][2] = 0;
    ret.data[2][0] = 0;
    ret.data[2][1] = 0;
    ret.data[2][2] = 1;

    if (size == 4){
        ret.data[0][3] = 0;
        ret.data[1][3] = 0;
        ret.data[2][3] = 0;
        ret.data[3][0] = 0;
        ret.data[3][1] = 0;
        ret.data[3][2] = 0;
        ret.data[3][3] = 1;
    }

    return ret;
}

Mat mat_scaling(float factor, int size) {
    Mat ret = create_matrix(size);
    
    ret.data[0][0] = factor;
    ret.data[0][1] = 0;
    ret.data[0][2] = 0;
    ret.data[1][0] = 0;
    ret.data[1][1] = factor;
    ret.data[1][2] = 0;
    ret.data[2][0] = 0;
    ret.data[2][1] = 0;
    ret.data[2][2] = factor;

    if (size == 4){
        ret.data[0][3] = 0;
        ret.data[1][3] = 0;
        ret.data[2][3] = 0;
        ret.data[3][0] = 0;
        ret.data[3][1] = 0;
        ret.data[3][2] = 0;
        ret.data[3][3] = 1;
    }
    
    return ret;
}

Mat mat_rotation_x(float angle, int size) {
    Mat ret = create_matrix(size);
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

    if (size == 4){
        ret.data[0][3] = 0;
        ret.data[1][3] = 0;
        ret.data[2][3] = 0;
        ret.data[3][0] = 0;
        ret.data[3][1] = 0;
        ret.data[3][2] = 0;
        ret.data[3][3] = 1;
    }

    return ret;
}

Mat mat_rotation_y(float angle, int size) {
    Mat ret = create_matrix(size);
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

    if (size == 4){
        ret.data[0][3] = 0;
        ret.data[1][3] = 0;
        ret.data[2][3] = 0;
        ret.data[3][0] = 0;
        ret.data[3][1] = 0;
        ret.data[3][2] = 0;
        ret.data[3][3] = 1;
    }

    return ret;
}

Mat mat_rotation_z(float angle, int size) {
    Mat ret = create_matrix(size);
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

    if (size == 4){
        ret.data[0][3] = 0;
        ret.data[1][3] = 0;
        ret.data[2][3] = 0;
        ret.data[3][0] = 0;
        ret.data[3][1] = 0;
        ret.data[3][2] = 0;
        ret.data[3][3] = 1;
    }

    return ret;
}

Mat mat_translation(float x, float y, float z) {
    Mat ret = create_matrix(4);
    
    ret.data[0][0] = 1;
    ret.data[0][1] = 0;
    ret.data[0][2] = 0;
    ret.data[0][3] = 0;
    ret.data[1][0] = 0;
    ret.data[1][1] = 1;
    ret.data[1][2] = 0;
    ret.data[1][3] = 0;
    ret.data[2][0] = 0;
    ret.data[2][1] = 0;
    ret.data[2][2] = 1;
    ret.data[2][3] = 0;
    ret.data[3][0] = x;
    ret.data[3][1] = y;
    ret.data[3][2] = z;
    ret.data[3][3] = 1;

    return ret;
}

Vec3 multiply_matrix_by_vec3(const Mat matrix, const Vec3* point) {
    Vec3 result;
    result.x = (point->x * matrix.data[0][0]) + (point->y * matrix.data[1][0]) + (point->z * matrix.data[2][0]);
    result.y = (point->x * matrix.data[0][1]) + (point->y * matrix.data[1][1]) + (point->z * matrix.data[2][1]);
    result.z = (point->x * matrix.data[0][2]) + (point->y * matrix.data[1][2]) + (point->z * matrix.data[2][2]);
    return result;
}

Vec4 multiply_matrix_by_vec4(const Mat matrix, const Vec4* point) {
    Vec4 result;
    result.x = (point->x * matrix.data[0][0]) + (point->y * matrix.data[1][0]) + (point->z * matrix.data[2][0]) + (point->w * matrix.data[3][0]);
    result.y = (point->x * matrix.data[0][1]) + (point->y * matrix.data[1][1]) + (point->z * matrix.data[2][1]) + (point->w * matrix.data[3][1]);
    result.z = (point->x * matrix.data[0][2]) + (point->y * matrix.data[1][2]) + (point->z * matrix.data[2][2]) + (point->w * matrix.data[3][2]);
    result.w = (point->x * matrix.data[0][3]) + (point->y * matrix.data[1][3]) + (point->z * matrix.data[2][3]) + (point->w * matrix.data[3][3]);
    return result;
}

Mat multiply_matrices(const Mat matrixA, const Mat matrixB) {
    int size = matrixA.size;
    Mat result = create_matrix(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result.data[i][j] = 0.0f;
            for (int k = 0; k < size; ++k) {
                result.data[i][j] += matrixA.data[i][k] * matrixB.data[k][j];
            }
        }
    }

    return result;
}
