#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include "vec3.h"
#include "vertex.h"

typedef struct IndexedTriangleList {
    Vertex* vertices;
    int sizeV;

    Vec3* indices;
    int sizeI;
} IndexedTriangleList;

#endif