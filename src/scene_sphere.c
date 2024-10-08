#include "scene.h"

int calcIdx(int iLat, int iLong, int latDiv, int longDiv) {
    return iLat * longDiv + iLong;
}

IndexedTriangleList* sphere_init_triangle_list(float radius, int latDiv, int longDiv) {
    // make vertices of a sphere
    const int sizeV = (latDiv - 1) * longDiv + 2;
    Vertex* vertices = malloc(sizeV * sizeof(Vertex));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1);
    }

    const Vec3 base = (Vec3){0.0f, 0.0f, radius};
    const float latitudeAngle = PI / latDiv;
    const float longitudeAngle = 2 * PI / longDiv;

    int i = 0;
    for (int iLat = 1; iLat < latDiv; iLat++) {

        Mat rotation_matrix_x = mat_rotation_x(latitudeAngle * iLat, 3);

        Vec3 latBase = multiply_matrix_by_vec3(rotation_matrix_x, &base);

        for (int iLong = 0; iLong < longDiv; iLong++) {
            Mat rotation_matrix_z = mat_rotation_z(longitudeAngle * iLong, 3);

            Vec3 vert = multiply_matrix_by_vec3(rotation_matrix_z, &latBase);

            vertices[i++].pos.as_vec3 = vert;
        }
    }

    // add poles
    const int iNorthPole = i;
    vertices[i++].pos.as_vec3 = base;

    const int iSouthPole = i;
    vertices[i++].pos.as_vec3 = (Vec3){0.0f, 0.0f, -radius}; // -base

    // make indices of a sphere
    int sizeI = (latDiv - 2) * (longDiv - 1) * 2 + longDiv * 3;
    Vec3* indices = malloc(sizeI * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices);
        exit(1);
    }
    i = 0;
    for (int iLat = 0; iLat < latDiv - 2; iLat++) {
        for (int iLong = 0; iLong < longDiv - 1; iLong++) {
            indices[i].x =    calcIdx(iLat, iLong + 1, latDiv, longDiv);
            indices[i].y =      calcIdx(iLat + 1, iLong, latDiv, longDiv);
            indices[i++].z =      calcIdx(iLat, iLong, latDiv, longDiv);

            indices[i].x =    calcIdx(iLat + 1, iLong + 1, latDiv, longDiv);
            indices[i].y =      calcIdx(iLat + 1, iLong, latDiv, longDiv);
            indices[i++].z =      calcIdx(iLat, iLong + 1, latDiv, longDiv);
        }
        indices[i].x =    calcIdx(iLat, 0, latDiv, longDiv);
        indices[i].y =      calcIdx(iLat + 1, longDiv - 1, latDiv, longDiv);
        indices[i++].z =      calcIdx(iLat, longDiv - 1, latDiv, longDiv);

        indices[i].x =    calcIdx(iLat + 1, 0, latDiv, longDiv);
        indices[i].y =      calcIdx(iLat + 1, longDiv - 1, latDiv, longDiv);
        indices[i++].z =      calcIdx(iLat, 0, latDiv, longDiv);
    }

    // conect poles
    for (int iLong = 0; iLong < longDiv - 1; iLong++) {
        indices[i].x =    calcIdx(0, iLong + 1, latDiv, longDiv);
        indices[i].y =      calcIdx(0, iLong, latDiv, longDiv);
        indices[i++].z =      iNorthPole;
        
        indices[i].x =    iSouthPole;
        indices[i].y =      calcIdx(latDiv - 2, iLong, latDiv, longDiv);
        indices[i++].z =      calcIdx(latDiv - 2, iLong + 1, latDiv, longDiv);
    }
    indices[i].x =    calcIdx(0, 0, latDiv, longDiv);
    indices[i].y =      calcIdx(0, longDiv - 1, latDiv, longDiv);
    indices[i++].z =      iNorthPole;

    indices[i].x =    iSouthPole;
    indices[i].y =      calcIdx(latDiv - 2, longDiv - 1, latDiv, longDiv);
    indices[i++].z =      calcIdx(latDiv - 2, 0, latDiv, longDiv);

    // can't figure out sizeI calculation, so just making it a bit bigger, and then copying i elements to new array
    sizeI = i;
    Vec3* indices_cpy = malloc(sizeI * sizeof(Vec3));
    memcpy(indices_cpy, indices, sizeI * sizeof(Vec3));
    free(indices);

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices_cpy);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = sizeV;
    triList->indices = indices_cpy;
    triList->sizeI = sizeI;

    return triList;
}

void sphere_init_normals(Scene* scene, float radius, int latDiv, int longDiv) {
    scene->triList = sphere_init_triangle_list(radius, latDiv, longDiv);
    for (int i = 0; i < scene->triList->sizeV; i++) {
        Vec3 norm = vec3_normalize(&scene->triList->vertices[i].pos.as_vec3);
        scene->triList->vertices[i].n = norm;
    }
}