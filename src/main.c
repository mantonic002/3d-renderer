#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constants.h"

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);

bool game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
float delta_time = 0.0f;

bool keys[SDL_NUM_SCANCODES] = { false };

TTF_Font* font = NULL;

int projection_matrix[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
};

// points of a cube
int points[8][3] = {
    {-1, -1, 1},
    {1, -1, 1},
    {1, 1, 1},
    {-1, 1, 1},
    {-1, -1, -1},
    {1, -1, -1},
    {1, 1, -1},
    {-1, 1, -1},
};

int main() {
    game_is_running = initialize_window();

    setup();
    
    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}


int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    
    if (TTF_Init() != 0) {
        fprintf(stderr, "Error initializing SDL_ttf.\n");
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
    );
    if (!renderer) {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    return true;
}

void setup() {
    font = TTF_OpenFont("src/font.ttf", FONT_SIZE);
    if (!font) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        destroy_window();
    }
}

void process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            game_is_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = false;
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

    // Update player positions based on key states
    // if (keys[SDL_SCANCODE_S])
    // if (keys[SDL_SCANCODE_W])
    // if (keys[SDL_SCANCODE_DOWN])
    // if (keys[SDL_SCANCODE_UP])
}



void update() {
    // Better way is to use proper delay function
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // delta time factor in seconds
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    // Update logic:
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //TODO: draw edges, add rotation 

    // Drawing the points
    for (int i = 0; i < 8; i++) {
        int projection[3];
        int* point = points[i];
        projection[0] = (point[0] * projection_matrix[0][0]) + (point[1] * projection_matrix[1][0]) + (point[2] * projection_matrix[2][0]);
        projection[1] = (point[0] * projection_matrix[0][1]) + (point[1] * projection_matrix[1][1]) + (point[2] * projection_matrix[2][1]);
        projection[2] = (point[0] * projection_matrix[0][2]) + (point[1] * projection_matrix[1][2]) + (point[2] * projection_matrix[2][2]);
        
        SDL_Rect rect = {WINDOW_WIDTH/2 + projection[0] * 100, WINDOW_HEIGHT/2 + projection[1] * 100, 5, 5};

        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_Color text_color = { 255, 255, 255 }; // White color
    // Render text
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, "SPINNING CUBE", text_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = { 0,0, text_surface->w, text_surface->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);


    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}