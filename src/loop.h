#ifndef LOOP_H
#define LOOP_H

#include "constants.h"
#include "scene.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FRAME_TARGET_TIME (1000 / FPS)

int initialize_window(SDL_Window** window, SDL_Renderer** renderer);
void setup(SDL_Window** window, SDL_Renderer** renderer);
int process_input(bool* keys, float* delta_time, Scene* scenes, int scenes_size, int* curr_scene);
void update(int *last_frame_time, float *delta_time);
void destroy_window(SDL_Window** window, SDL_Renderer** renderer);

#endif