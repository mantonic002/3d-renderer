#include "./loop.h"

int initialize_window(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Error initializing SDL_image.\n");
        return false;
    }

    *window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!*window) {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        0
    );
    if (!*renderer) {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    return true;
}

void setup(SDL_Window** window, SDL_Renderer** renderer) {

}

int process_input(bool* keys, float* delta_time, Scene* scenes, int scenes_size, int* curr_scene) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;
            else if (event.key.keysym.sym == SDLK_TAB) {
                *curr_scene = (*curr_scene + 1)  % scenes_size;
            }
            else
                keys[event.key.keysym.scancode] = true;
            break;

        case SDL_KEYUP:
            keys[event.key.keysym.scancode] = false;
            break;

        default:
            break;
        }
    }

    // Update angles based on key states
    if (keys[SDL_SCANCODE_X])
        scenes[*curr_scene].angle_x += 0.02f;
    if (keys[SDL_SCANCODE_Y])
        scenes[*curr_scene].angle_y += 0.02f;
    if (keys[SDL_SCANCODE_Z])
        scenes[*curr_scene].angle_z += 0.02f;

    // move the cube closer or further away with arrow keys
    if (keys[SDL_SCANCODE_UP] && scenes[*curr_scene].z_offset >= 2 && scenes[*curr_scene].z_offset < 19.9f)
    {
        scenes[*curr_scene].z_offset += 0.1f;
    }

    if (keys[SDL_SCANCODE_DOWN] && scenes[*curr_scene].z_offset > 2.1f && scenes[*curr_scene].z_offset <= 20)
    {
        scenes[*curr_scene].z_offset -= 0.1f;
    }


    //check if rotation angles are more than 2pi and return them to zero if they are
    if (scenes[*curr_scene].angle_x >= 2*PI)
        scenes[*curr_scene].angle_x = 0;
    if (scenes[*curr_scene].angle_y >= 2*PI)
        scenes[*curr_scene].angle_y = 0;
    if (scenes[*curr_scene].angle_z >= 2*PI)
        scenes[*curr_scene].angle_z = 0;
    return true;
}

void update(int *last_frame_time, float *delta_time) {
    // delaying to keep the constant frame rate
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - *last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // delta time factor in seconds
    *delta_time = (SDL_GetTicks() - *last_frame_time) / 1000.0f;

    *last_frame_time = SDL_GetTicks();

}

void render(SDL_Renderer** renderer, Scene* scene) {
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);

    scene->draw(scene, renderer);

    SDL_RenderPresent(*renderer);
}

void destroy_window(SDL_Window** window, SDL_Renderer** renderer) {
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    IMG_Quit();
    SDL_Quit();
}