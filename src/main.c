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

    SDL_Surface* textureFromPng = NULL;

    game_is_running = initialize_window(&window, &renderer);

    setup(&window, &renderer);

    Scene cube = make_cube(&textureFromPng, 1.0f);

    while (game_is_running) {
        if (!process_input(keys, &delta_time, &cube)) game_is_running = false;
        update(&last_frame_time, &delta_time, &cube);
        render(&renderer, &cube);
    }

    destroy_window(&window, &renderer);

    return 0;
}