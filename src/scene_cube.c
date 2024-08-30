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
    float rotation_matrix_z[3][3] = {
        {cos(scene->angle_z), -sin(scene->angle_z), 0},
        {sin(scene->angle_z), cos(scene->angle_z), 0},
        {0, 0, 1},
    };

    float rotation_matrix_y[3][3] = {
        {cos(scene->angle_y), 0, sin(scene->angle_y)},
        {0, 1, 0},
        {-sin(scene->angle_y), 0, cos(scene->angle_y)},
    };

    float rotation_matrix_x[3][3] = {
        {1, 0, 0},
        {0, cos(scene->angle_x), -sin(scene->angle_x)},
        {0, sin(scene->angle_x), cos(scene->angle_x)},
    };

    // multiply all 3 rotation matrices to get a final rotation matrix
    float temp[3][3];
    multiply_matrices(rotation_matrix_x, rotation_matrix_y, temp);
    float rotation[3][3];
    multiply_matrices(temp, rotation_matrix_z, rotation);

    // get translation
    Vec3 trans = {0.0f, 0.0f, scene->z_offset};

    // set pipeline vertex shader
    scene->pipeline->vertex_shader =  create_default_vertex_shader(rotation, &trans);

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}