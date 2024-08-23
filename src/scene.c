#include "scene.h"

Scene make_cube(SDL_Surface** texture, float texture_dimension) {
    // vertices of a cube
    Vertex* vertices = malloc(8 * sizeof(Vertex));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1);
    }

    Vertex temp_vertices[8] = {
        {{-SIZE, -SIZE, -SIZE},  {0.0f, texture_dimension}},            
        {{SIZE, -SIZE, -SIZE},   {texture_dimension, texture_dimension}},
        {{-SIZE, SIZE, -SIZE},   {0.0f, 0.0f}},
        {{SIZE, SIZE, -SIZE},    {texture_dimension, 0.0f}},            
        {{-SIZE, -SIZE, SIZE},   {texture_dimension, texture_dimension}},
        {{SIZE, -SIZE, SIZE},    {0.0f, texture_dimension}},            
        {{-SIZE, SIZE, SIZE},     {texture_dimension, 0.0f}},            
        {{SIZE, SIZE, SIZE},     {0.0f, 0.0f}},
    };
    memcpy(vertices, temp_vertices, 8 * sizeof(Vertex));

    Vec3* indices = malloc(12 * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices); // Free previously allocated memory
        exit(1);
    }

    Vec3 temp_indices[12] = {
        {0, 2, 1}, {2, 3, 1},
        {1, 3, 5}, {3, 7, 5},
        {2, 6, 3}, {3, 6, 7},
        {7, 4, 5}, {4, 7, 6},
        {0, 4, 2}, {2, 4, 6},
        {0, 1, 4}, {1, 5, 4},
    };
    memcpy(indices, temp_indices, 12 * sizeof(Vec3));

    float angle_x = 0;
    float angle_y = 0;
    float angle_z = 0;

    float z_offset = 3;

    *texture = IMG_Load("res/texture.png");
    if (!*texture) {
        fprintf(stderr, "Unable to load image! SDL_Error: %s\n", SDL_GetError());
        free(vertices); // Free previously allocated memory
        free(indices);
        exit(1);
    }

    Scene cube = {
        vertices,
        indices,
        angle_x,
        angle_y,
        angle_z,
        z_offset,
        *texture,
    };

    return cube;
}

void restart_cube(Scene* scene) {
    scene->vertices[0].pos.x = -SIZE; scene->vertices[0].pos.y = -SIZE; scene->vertices[0].pos.z = -SIZE;
    scene->vertices[1].pos.x = SIZE;  scene->vertices[1].pos.y = -SIZE; scene->vertices[1].pos.z = -SIZE;
    scene->vertices[2].pos.x = -SIZE; scene->vertices[2].pos.y = SIZE;  scene->vertices[2].pos.z = -SIZE;
    scene->vertices[3].pos.x = SIZE;  scene->vertices[3].pos.y = SIZE;  scene->vertices[3].pos.z = -SIZE;
    scene->vertices[4].pos.x = -SIZE; scene->vertices[4].pos.y = -SIZE; scene->vertices[4].pos.z = SIZE;
    scene->vertices[5].pos.x = SIZE;  scene->vertices[5].pos.y = -SIZE; scene->vertices[5].pos.z = SIZE;
    scene->vertices[6].pos.x = -SIZE; scene->vertices[6].pos.y = SIZE;  scene->vertices[6].pos.z = SIZE;
    scene->vertices[7].pos.x = SIZE;  scene->vertices[7].pos.y = SIZE;  scene->vertices[7].pos.z = SIZE;
}