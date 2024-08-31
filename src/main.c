#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loop.h"
#include "helper.h"
#include "scene.h"

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
    cube_flat_shade.draw = scene_cube_flat_shade_draw;
    cube_init_triangle_list_independent_faces_w_normals(&cube_flat_shade);

    Scene model = make_scene_model(&renderer, "res/cat.png");
    model.draw = scene_model_draw;
    model_init_triangle_list(&model, "res/cat.obj");

    Scene scenes[6] = {
        model,
        cube,
        cube_skinned,
        double_cube,
        wave_plane,
        cube_flat_shade,
    };

    int curr_scene = 0;

    while (game_is_running) {
        if (!process_input(keys, &delta_time, scenes, 6, &curr_scene)) game_is_running = false;
        update(&last_frame_time, &delta_time);
        render(&renderer, &scenes[curr_scene]);
    }

    destroy_window(&window, &renderer);

    return 0;
}