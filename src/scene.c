#include "scene.h"

Scene make_cube(SDL_Surface** texture) {
    bool wireframe = false;
    bool textured = false;

    // vertices of a cube
    Vec3* vertices = malloc(8 * sizeof(Vec3));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1); // Or handle the error as needed
    }

    Vec3 temp_vertices[8] = {
        {-SIZE, -SIZE, -SIZE},
        {SIZE, -SIZE, -SIZE},
        {-SIZE, SIZE, -SIZE},
        {SIZE, SIZE, -SIZE}, 
        {-SIZE, -SIZE, SIZE},
        {SIZE, -SIZE, SIZE}, 
        {-SIZE, SIZE, SIZE}, 
        {SIZE, SIZE, SIZE},  
    };
    memcpy(vertices, temp_vertices, 8 * sizeof(Vec3));

    TexVertex* projected_points = malloc(8 * sizeof(TexVertex));
    if (!projected_points) {
        fprintf(stderr, "Memory allocation failed for projected_points\n");
        free(vertices); // Free previously allocated memory
        exit(1); // Or handle the error as needed
    }

    TexVertex temp_projected_points[8] = {
        {{-SIZE, -SIZE}, {0.0f, 1.0f}},
        {{SIZE, -SIZE},  {1.0f, 1.0f}},
        {{-SIZE, SIZE},  {0.0f, 0.0f}},
        {{SIZE, SIZE},   {1.0f, 0.0f}},
        {{-SIZE, -SIZE},  {1.0f, 1.0f}},
        {{SIZE, -SIZE},   {0.0f, 1.0f}},
        {{-SIZE, SIZE},   {1.0f, 0.0f}},
        {{SIZE, SIZE},    {0.0f, 0.0f}},
    };
    memcpy(projected_points, temp_projected_points, 8 * sizeof(TexVertex));

    Vec2* edges = malloc(12 * sizeof(Vec2));
    if (!edges) {
        fprintf(stderr, "Memory allocation failed for edges\n");
        free(vertices); // Free previously allocated memory
        free(projected_points);
        exit(1); // Or handle the error as needed
    }

    Vec2 temp_edges[12] = {
        {0, 1}, {1, 3}, 
        {3, 2}, {2, 0}, 
        {0, 4}, {1, 5}, 
        {3, 7}, {2, 6}, 
        {4, 5}, {5, 7}, 
        {7, 6}, {6, 4}, 
    };
    memcpy(edges, temp_edges, 12 * sizeof(Vec2));

    Vec3* indices = malloc(12 * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices); // Free previously allocated memory
        free(projected_points);
        free(edges);
        exit(1); // Or handle the error as needed
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

    bool* cullFlags = malloc(12 * sizeof(bool));
    if (!cullFlags) {
        fprintf(stderr, "Memory allocation failed for cullFlags\n");
        free(vertices); // Free previously allocated memory
        free(projected_points);
        free(edges);
        free(indices);
        exit(1); // Or handle the error as needed
    }
    memset(cullFlags, 0, 12 * sizeof(bool)); // Initialize to false

    float angle_x = 0;
    float angle_y = 0;
    float angle_z = 0;

    float z_offset = 3;

    *texture = IMG_Load("res/texture.png");
    if (!*texture) {
        fprintf(stderr, "Unable to load image! SDL_Error: %s\n", SDL_GetError());
        free(vertices); // Free previously allocated memory
        free(projected_points);
        free(edges);
        free(indices);
        free(cullFlags);
        exit(1); // Or handle the error as needed
    }

    Scene cube = {
        wireframe, textured,
        vertices,
        projected_points,
        edges,
        indices,
        cullFlags,
        angle_x,
        angle_y,
        angle_z,
        z_offset,
        *texture,
    };

    return cube;
}

void restart_cube(Scene* scene) {
    scene->vertices[0].x = -SIZE; scene->vertices[0].y = -SIZE; scene->vertices[0].z = -SIZE;
    scene->vertices[1].x = SIZE;  scene->vertices[1].y = -SIZE; scene->vertices[1].z = -SIZE;
    scene->vertices[2].x = -SIZE; scene->vertices[2].y = SIZE;  scene->vertices[2].z = -SIZE;
    scene->vertices[3].x = SIZE;  scene->vertices[3].y = SIZE;  scene->vertices[3].z = -SIZE;
    scene->vertices[4].x = -SIZE; scene->vertices[4].y = -SIZE; scene->vertices[4].z = SIZE;
    scene->vertices[5].x = SIZE;  scene->vertices[5].y = -SIZE; scene->vertices[5].z = SIZE;
    scene->vertices[6].x = -SIZE; scene->vertices[6].y = SIZE;  scene->vertices[6].z = SIZE;
    scene->vertices[7].x = SIZE;  scene->vertices[7].y = SIZE;  scene->vertices[7].z = SIZE;
}