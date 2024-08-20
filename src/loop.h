#ifndef LOOP_H
#define LOOP_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./scene.h"

#define FRAME_TARGET_TIME (1000 / FPS)

int initialize_window(SDL_Window** window, SDL_Renderer** renderer);
void setup(TTF_Font** font, SDL_Window** window, SDL_Renderer** renderer);
int process_input(bool* keys, float* delta_time, Scene* scene);
void update(int *last_frame_time, float *delta_time, Scene* scene);
void destroy_window(SDL_Window** window, SDL_Renderer** renderer);

#endif