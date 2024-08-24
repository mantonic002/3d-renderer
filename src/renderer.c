#include "renderer.h"

void render(SDL_Renderer** renderer, Scene* scene) {
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);

    // TODO: Pipeline and all it's fields are being created every frame as is, needs to be moved
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

    float temp[3][3];
    multiply_matrices(rotation_matrix_x, rotation_matrix_y, temp);


    float rotation[3][3];
    multiply_matrices(temp, rotation_matrix_z, rotation);

    Vec3 trans = {0.0f, 0.0f, scene->z_offset};

    PixelShader* shader = create_pixel_shader("res/texture.png");

    Pipeline p;
    p.renderer = renderer;
    p.translation = trans;
    p.pixel_shader = shader;
    pipeline_bind_rotation(&p, rotation);

    IndexedTriangleList i = {
        scene->vertices,
        8,  
        scene->indices,
        12,
    };
    pipeline_draw(&p, &i);

    SDL_RenderPresent(*renderer);
}