#include "renderer.h"

void render(SDL_Renderer** renderer, Scene* scene) {
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);

    float projection_matrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    };

    // rotation matrices
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

    float zInv;

    //TODO: Everything in this loops keeps repeating for some reason
    // rotate and make a projection of each point
    for (int i = 0; i < 8; i++) {
        Vec3 point = scene->vertices[i].pos;

        // rotation around z axis
        Vec3 rotation_z;
        multiplyMatrixByPoint(rotation_matrix_z, &point, &rotation_z);
        
        // rotation around y axis
        Vec3 rotation_y;
        multiplyMatrixByPoint(rotation_matrix_y, &rotation_z, &rotation_y);

        // rotation around x axis
        Vec3 rotation_x;
        multiplyMatrixByPoint(rotation_matrix_x, &rotation_y, &rotation_x);

        // move cube away from the screen by z_offset
        rotation_x.z += scene->z_offset;

        scene->vertices[i].pos = rotation_x;
    }

    // backface culling
    for (int i = 0; i < 12; i++) {
        Vec3 v1 = scene->vertices[(int)scene->indices[i].x].pos;
        Vec3 v2 = scene->vertices[(int)scene->indices[i].y].pos;
        Vec3 v3 = scene->vertices[(int)scene->indices[i].z].pos;

        Vec3 v2_sub_v1 = vec3_subtract(&v2, &v1);
        Vec3 v3_sub_v1 = vec3_subtract(&v3, &v1);
        Vec3 n = cross_product(&v2_sub_v1, &v3_sub_v1);
        scene->cullFlags[i] =  dot_product(&n, &v1) > 0.0f;
    }

    // projection
    for (int i = 0; i < 8; i++) {
        Vec3 curr = scene->vertices[i].pos;
        
        zInv = 1.0f / curr.z;
        curr.x = curr.x*zInv;
        curr.y = -curr.y*zInv;

        Vec3 projection;
        multiplyMatrixByPoint(projection_matrix, &curr, &projection);

        scene->vertices[i].pos.x = WINDOW_WIDTH/2 + curr.x * SCALE;
        scene->vertices[i].pos.y = WINDOW_HEIGHT/2 + curr.y * SCALE;
    }

    // draw lines based on the edges and vertices arrays
    if (scene->wireframe) {
        for (int i = 0; i < 12; i++) {
            Vec2 edge = scene->edges[i];
            Vec3 p1 = scene->vertices[(int)edge.x].pos;
            Vec3 p2 = scene->vertices[(int)edge.y].pos;

            SDL_RenderDrawLine(*renderer, p1.x, p1.y, p2.x, p2.y);
        }
    }
    else if (scene->textured) {
        for (int i = 0; i < 12; i++) {
            if (!scene->cullFlags[i]) {                
                Vec3 index = scene->indices[i];
                Vertex tv1 = scene->vertices[(int)index.x];
                Vertex tv2 = scene->vertices[(int)index.y];
                Vertex tv3 = scene->vertices[(int)index.z];

                draw_triangle_tex(&tv1, &tv2, &tv3, scene->texture, renderer);
            }
        }
    }
    else {
        SDL_Color colors[12] = {
            { 125, 125, 125, 255 },
            { 255, 0, 0, 255 },
            { 0, 0, 255, 255 },
            { 100, 25, 124, 255 },
            { 255, 255, 255, 255 },
            { 0, 255, 255, 255 },
            { 255, 15, 0, 100 },
            { 255, 255, 0, 255 },
            { 124, 124, 0, 255 },
            { 255, 2, 255, 255 },
            { 0, 124, 124, 255 },
            { 255, 255, 255, 125 },
        };

        // draw triangles
        for (int i = 0; i < 12; i++) {
            if (!scene->cullFlags[i]) {                
                Vec3 index = scene->indices[i];
                Vec3 v1 = scene->vertices[(int)index.x].pos;
                Vec3 v2 = scene->vertices[(int)index.y].pos;
                Vec3 v3 = scene->vertices[(int)index.z].pos;

                draw_triangle(&v1, &v2, &v3, colors[i], renderer);
            }
        }
    }

    restart_cube(scene);

    SDL_RenderPresent(*renderer);
}