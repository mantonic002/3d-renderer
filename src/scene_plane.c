#include "scene.h"

int compute_indices(int x, int y, int nVerticesSide) {
    return y * nVerticesSide + x;
}

void plane_init_triangle_list(Scene* scene, int divisions, float size) {
    int nVerticesSide = divisions + 1;
    int numVertices = SQUARE(nVerticesSide);
    int numIndices = SQUARE(divisions) * 2;

    // Allocate memory for vertices and indices
    Vertex* vertices = (Vertex *)malloc(numVertices * sizeof(Vertex));
    Vec3* indices = (Vec3 *)malloc(numIndices * sizeof(Vec3));

    if (!vertices || !indices) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed for indices or indices\n");
        exit(1);
    }

    float side = size / 2.0f;
    float divisionSize = size / (float)divisions;
    Vec3 bottomLeft = {-side, -side, 0.0f};

    // Compute vertex positions
    for (int y = 0, i = 0; y < nVerticesSide; y++) {
        float y_pos = (float)y * divisionSize;
        for (int x = 0; x < nVerticesSide; x++, i++) {
            vertices[i].pos = (Vec3){
                bottomLeft.x + (float)x * divisionSize,
                bottomLeft.y + y_pos,
                0.0f
            };
        }
    }

    // Compute indices
    int index = 0;
    for (int y = 0; y < divisions; y++) {
        for (int x = 0; x < divisions; x++) {
            int indexArray[4] = {
                compute_indices(x, y, nVerticesSide),
                compute_indices(x + 1, y, nVerticesSide),
                compute_indices(x, y + 1, nVerticesSide),
                compute_indices(x + 1, y + 1, nVerticesSide)
            };

            indices[index].x = indexArray[0];
            indices[index].y = indexArray[2];
            indices[index++].z = indexArray[1];
            indices[index].x = indexArray[1];
            indices[index].y = indexArray[2];
            indices[index++].z = indexArray[3];
        }
    }

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = numVertices;
    triList->indices = indices;
    triList->sizeI = numIndices;

    scene->triList = triList;
}

void plane_init_triangle_list_skinned(Scene* scene, int divisions, float size) {
    plane_init_triangle_list(scene, divisions, size);

    const int nVerticesSide = divisions + 1;
    const float tDivisionSize = 1.0f / (float)divisions;
    const Vec2 tBottomLeft = { 0.0f,1.0f };

    for( int y = 0,i = 0; y < nVerticesSide; y++ )
    {
        const float y_t = (float)-y * tDivisionSize;
        for( int x = 0; x < nVerticesSide; x++,i++ )
        {
            Vec2 temp = { (float)x * tDivisionSize,y_t };
            scene->triList->vertices[i].tc = vec2_add(&tBottomLeft, &temp);
        }
    }

}

void scene_plane_draw(Scene* scene, SDL_Renderer** renderer) {
    scene->time += 0.01f;
    scene->angle_x = PI/4;
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

    // rotation matrices for each axis
    Mat3 rotation_matrix_z = mat3_rotation_z(scene->angle_z);
    Mat3 rotation_matrix_y = mat3_rotation_y(scene->angle_y);
    Mat3 rotation_matrix_x = mat3_rotation_x(scene->angle_x);

    // multiply all 3 rotation matrices to get a final rotation matrix
    Mat3 rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
    rotation = multiply_matrices(rotation, rotation_matrix_z);

    // get translation
    Vec3 trans = {0.0f, 0.0f, scene->z_offset};

    // set pipeline vertex shader
    scene->pipeline->vertex_shader =  create_wave_vertex_shader(rotation.data, &trans, scene->time);

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}