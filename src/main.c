#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loop.h"
#include "helper.h"
#include "scene.h"
#include "renderer.h"


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

    Scene scenes[3] = {
        cube,
        cube_skinned,
        double_cube,
    };

    int curr_scene = 0;

    while (game_is_running) {
        if (!process_input(keys, &delta_time, scenes, 3, &curr_scene)) game_is_running = false;
        update(&last_frame_time, &delta_time);
        render(&renderer, &scenes[curr_scene]);
    }

    destroy_window(&window, &renderer);

    return 0;
}