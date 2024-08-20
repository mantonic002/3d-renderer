#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "./constants.h"
#include "./loop.h"
#include "./vec2.h"
#include "./vec3.h"
#include "scene.h"
#include "./tex_vertex.h"
#include "./draw_triangles.h"
#include "./draw_triangles_tex.h"



void render(SDL_Renderer** renderer, Scene* scene);
void restart_cube(Scene* scene);



int main() {
    bool game_is_running = false;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int last_frame_time = 0;
    float delta_time = 0.0f;

    bool keys[SDL_NUM_SCANCODES] = { false };

    TTF_Font* font = NULL;
    SDL_Surface* textureFromPng = NULL;

    game_is_running = initialize_window(&window, &renderer);

    setup(&font, &window, &renderer);

    Scene cube = make_cube(&textureFromPng);

    printf("%d", game_is_running);
    while (game_is_running) {
        if (!process_input(keys, &delta_time, &cube)) game_is_running = false;
        update(&last_frame_time, &delta_time, &cube);
        render(&renderer, &cube);
    }

    destroy_window(&window, &renderer);

    return 0;
}




void multiplyMatrixByPoint(const float matrix[3][3], const Vec3* point, Vec3* result) {
    result->x = (point->x * matrix[0][0]) + (point->y * matrix[1][0]) + (point->z * matrix[2][0]);
    result->y = (point->x * matrix[0][1]) + (point->y * matrix[1][1]) + (point->z * matrix[2][1]);
    result->z = (point->x * matrix[0][2]) + (point->y * matrix[1][2]) + (point->z * matrix[2][2]);
}

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
        Vec3 point = scene->vertices[i];

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

        scene->vertices[i] = rotation_x;
    }

    // backface culling
    for (int i = 0; i < 12; i++) {
        Vec3 v1 = scene->vertices[(int)scene->indices[i].x];
        Vec3 v2 = scene->vertices[(int)scene->indices[i].y];
        Vec3 v3 = scene->vertices[(int)scene->indices[i].z];

        Vec3 v2_sub_v1 = vec3_subtract(&v2, &v1);
        Vec3 v3_sub_v1 = vec3_subtract(&v3, &v1);
        Vec3 n = cross_product(&v2_sub_v1, &v3_sub_v1);
        scene->cullFlags[i] =  dot_product(&n, &v1) > 0.0f;
    }

    // projection
    for (int i = 0; i < 8; i++) {
        Vec3 curr = scene->vertices[i];
        
        zInv = 1.0f / curr.z;
        curr.x = curr.x*zInv;
        curr.y = -curr.y*zInv;

        Vec3 projection;
        multiplyMatrixByPoint(projection_matrix, &curr, &projection);

        scene->projected_points[i].pos.x = WINDOW_WIDTH/2 + curr.x * SCALE;
        scene->projected_points[i].pos.y = WINDOW_HEIGHT/2 + curr.y * SCALE;
    }

    restart_cube(scene);

    // draw lines based on the edges and vertices arrays
    if (scene->wireframe) {
        for (int i = 0; i < 12; i++) {
            Vec2 edge = scene->edges[i];
            Vec2 p1 = scene->projected_points[(int)edge.x].pos;
            Vec2 p2 = scene->projected_points[(int)edge.y].pos;

            SDL_RenderDrawLine(*renderer, p1.x, p1.y, p2.x, p2.y);
        }
    }
    else if (scene->textured) {
        for (int i = 0; i < 12; i++) {
            if (!scene->cullFlags[i]) {                
                Vec3 index = scene->indices[i];
                TexVertex tv1 = scene->projected_points[(int)index.x];
                TexVertex tv2 = scene->projected_points[(int)index.y];
                TexVertex tv3 = scene->projected_points[(int)index.z];

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
                Vec2 v1 = scene->projected_points[(int)index.x].pos;
                Vec2 v2 = scene->projected_points[(int)index.y].pos;
                Vec2 v3 = scene->projected_points[(int)index.z].pos;

                draw_triangle(&v1, &v2, &v3, colors[i], renderer);
            }
        }
    }


    // SDL_Color text_color = { 255, 255, 255 }; // White color
    // // Render text
    // SDL_Surface* text_surface = TTF_RenderText_Blended(font, "SPIN THE CUBE", text_color);
    // SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    // SDL_Rect text_rect = { 180, 0, text_surface->w, text_surface->h };
    // SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    // SDL_FreeSurface(text_surface);
    // SDL_DestroyTexture(text_texture);


    SDL_RenderPresent(*renderer);
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


