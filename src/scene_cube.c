#include "scene.h"

void cube_init_triangle_list(Scene* scene, float texture_dimension) {
    // make vertices of a cube
    const int sizeV = 8;
    Vertex* vertices = malloc(sizeV * sizeof(Vertex));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1);
    }
    Vertex temp_vertices[] = {
        {{-SIZE, -SIZE, -SIZE},  {0.0f, texture_dimension}},            
        {{SIZE, -SIZE, -SIZE},   {texture_dimension, texture_dimension}},
        {{-SIZE, SIZE, -SIZE},   {0.0f, 0.0f}},
        {{SIZE, SIZE, -SIZE},    {texture_dimension, 0.0f}},            
        {{-SIZE, -SIZE, SIZE},   {texture_dimension, texture_dimension}},
        {{SIZE, -SIZE, SIZE},    {0.0f, texture_dimension}},            
        {{-SIZE, SIZE, SIZE},     {texture_dimension, 0.0f}},            
        {{SIZE, SIZE, SIZE},     {0.0f, 0.0f}},
    };
    memcpy(vertices, temp_vertices, sizeV * sizeof(Vertex));

    // make indices of a cube
    const int sizeI = 12;
    Vec3* indices = malloc(sizeI * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices);
        exit(1);
    }
    Vec3 temp_indices[] = {
        {0, 2, 1}, {2, 3, 1},
        {1, 3, 5}, {3, 7, 5},
        {2, 6, 3}, {3, 6, 7},
        {7, 4, 5}, {4, 7, 6},
        {0, 4, 2}, {2, 4, 6},
        {0, 1, 4}, {1, 5, 4},
    };
    memcpy(indices, temp_indices, sizeI * sizeof(Vec3));

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = sizeV;
    triList->indices = indices;
    triList->sizeI = sizeI;

    scene->triList = triList;
}

void cube_init_triangle_list_skinned(Scene* scene) {
    // make vertices of a cube
    const int sizeV = 14;
    Vertex* vertices = malloc(sizeV * sizeof(Vertex));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1);
    }
    Vertex temp_vertices[] = {
        {{-SIZE, -SIZE, -SIZE},  convert_tex_coord(1.0f, 0.0f)},            
        {{SIZE, -SIZE, -SIZE},   convert_tex_coord(0.0f, 0.0f)}, 
        {{-SIZE, SIZE, -SIZE},   convert_tex_coord(1.0f, 1.0f)}, 
        {{SIZE, SIZE, -SIZE},    convert_tex_coord(0.0f, 1.0f)}, 
        {{-SIZE, -SIZE, SIZE},   convert_tex_coord(1.0f, 3.0f)}, 
        {{SIZE, -SIZE, SIZE},    convert_tex_coord(0.0f, 3.0f)}, 
        {{-SIZE, SIZE, SIZE},    convert_tex_coord(1.0f, 2.0f)}, 
        {{SIZE, SIZE, SIZE},     convert_tex_coord(0.0f, 2.0f)}, 
        {{-SIZE, -SIZE, -SIZE},  convert_tex_coord(1.0f, 4.0f)}, 
        {{SIZE, -SIZE, -SIZE},   convert_tex_coord(0.0f, 4.0f)}, 
        {{-SIZE, -SIZE, -SIZE},  convert_tex_coord(2.0f, 1.0f)}, 
        {{-SIZE, -SIZE, SIZE},   convert_tex_coord(2.0f, 2.0f)}, 
        {{SIZE, -SIZE, -SIZE},   convert_tex_coord(-1.0f, 1.0f)}, 
        {{SIZE, -SIZE, SIZE},    convert_tex_coord(-1.0f, 2.0f)}, 
    };
    memcpy(vertices, temp_vertices, sizeV * sizeof(Vertex));

    // make indices of a cube
    const int sizeI = 12;
    Vec3* indices = malloc(sizeI * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices);
        exit(1);
    }
    Vec3 temp_indices[] = {
        {0, 2, 1}, {2, 3, 1},
        {4, 8, 5}, {5, 8, 9},
        {2, 6, 3}, {3, 6, 7},
        {4, 5, 7}, {4, 7, 6},
        {2, 10, 11}, {2, 11, 6},
        {12, 3, 7}, {12, 7, 13},
    };
    memcpy(indices, temp_indices, sizeI * sizeof(Vec3));

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = sizeV;
    triList->indices = indices;
    triList->sizeI = sizeI;

    scene->triList = triList;
}



void scene_cube_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

    // rotation matrices for each axis
    Mat rotation_matrix_z = mat_rotation_z(scene->angle_z, 3);
    Mat rotation_matrix_y = mat_rotation_y(scene->angle_y, 3);
    Mat rotation_matrix_x = mat_rotation_x(scene->angle_x, 3);

    // multiply all 3 rotation matrices to get a final rotation matrix
    Mat rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
    rotation = multiply_matrices(rotation, rotation_matrix_z);

    // set pipeline vertex shader
    memcpy(&scene->pipeline->vertex_shader->rotation, &rotation, sizeof(Mat));
    scene->pipeline->vertex_shader->translation = (Vec3){0.0f, 0.0f, scene->z_offset};

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}