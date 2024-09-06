#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loop.h"
#include "helper.h"
#include "scene.h"
#include "obj_loader.h"

int main() {
    bool game_is_running = false;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int last_frame_time = 0;
    float delta_time = 0.0f;

    bool keys[SDL_NUM_SCANCODES] = { false };

    game_is_running = initialize_window(&window, &renderer);

    setup(&window, &renderer);

    // initialize scenes
    Scene cube = make_scene(&renderer, "res/texture.png");
    cube.draw = scene_cube_draw;
    cube_init_triangle_list(&cube, 1.0f);

    Scene cube_skinned = make_scene(&renderer, "res/dice_skin.png");
    cube_skinned.draw = scene_cube_draw;
    cube_init_triangle_list_skinned(&cube_skinned);

    Scene double_cube = make_scene(&renderer, "res/dice_skin.png");
    double_cube.draw = scene_double_cube_draw;
    cube_init_triangle_list_skinned(&double_cube);

    Scene wave_plane = make_scene(&renderer, "res/texture.png");
    wave_plane.draw = scene_plane_draw;
    plane_init_triangle_list_skinned(&wave_plane, 20, 3.0f);

    Scene cube_flat_shade = make_scene_flat_shade(&renderer);
    cube_flat_shade.draw = scene_flat_shade_draw;
    cube_init_triangle_list_independent_faces_w_normals(&cube_flat_shade);

    Scene sphere_flat_shade = make_scene_flat_shade(&renderer);
    sphere_flat_shade.draw = scene_flat_shade_draw;
    sphere_init_normals(&sphere_flat_shade, 1.0f, 16, 32);

    Scene cat = make_scene_point_light_per_vertex(&renderer, "res/cat.png");
    cat.draw = scene_point_light_per_vertex_draw;
    load_obj(&cat, "res/cat.obj");

    Scene shark = make_scene_point_light_per_vertex(&renderer, "");
    shark.draw = scene_point_light_per_vertex_draw;
    load_obj(&shark, "res/shark.obj");

    Scene cube_per_pixel_light = make_scene_point_light_per_pixel(&renderer, "");
    cube_per_pixel_light.draw = scene_point_light_per_pixel_draw;
    cube_init_triangle_list_independent_faces_w_normals(&cube_per_pixel_light);

    Scene scenes[9] = {
        cube_per_pixel_light,
        shark,
        cat,
        sphere_flat_shade,
        cube_flat_shade,
        wave_plane,
        double_cube,
        cube,
        cube_skinned,
    };

    int curr_scene = 0;

    while (game_is_running) {
        if (!process_input(keys, &delta_time, scenes, 9, &curr_scene)) game_is_running = false;
        update(&last_frame_time, &delta_time);
        render(&renderer, &scenes[curr_scene]);
    }

    destroy_window(&window, &renderer);

    return 0;
}