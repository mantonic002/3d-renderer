#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
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

// points of a cube
float points[8][3] = {
    {-1, -1, 1}, //close bottom left    0
    {1, -1, 1}, //close bottom right    1
    {1, 1, 1}, //close top right        2
    {-1, 1, 1}, //close top left        3
    {-1, -1, -1}, //far bottom left     4
    {1, -1, -1}, //far bottom right     5
    {1, 1, -1}, //far top right         6
    {-1, 1, -1}, //far top left         7
};

float projected_points[8][3] = {0};

// array for storing edges of the cube
int edges[12][2] = {
    {0, 1}, {1, 2}, 
    {2, 3}, {3, 0}, 
    {4, 5}, {5, 6}, 
    {6, 7}, {7, 4}, 
    {0, 4}, {1, 5}, 
    {2, 6}, {3, 7}, 
};

float angle_x = 0;
float angle_y = 0;
float angle_z = 0;

float z_offset = 3;

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

    // Update angles based on key states
    if (keys[SDL_SCANCODE_X])
        angle_x += 0.02f;
    if (keys[SDL_SCANCODE_Y])
        angle_y += 0.02f;
    if (keys[SDL_SCANCODE_Z])
        angle_z += 0.02f;

    // move the cube closer or further away with arrow keys
    if (keys[SDL_SCANCODE_UP] && z_offset >= 2 && z_offset < 19.9f)
    {
        z_offset += 0.05f;
    }

    if (keys[SDL_SCANCODE_DOWN] && z_offset > 2.1f && z_offset <= 20)
    {
        z_offset -= 0.05f;
    }

}

void update() {
    // delaying to keep the constant frame rate
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // delta time factor in seconds
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    //check if rotation angles are more than 2pi and return them to zero if they are
    if (angle_x >= 2*PI)
        angle_x = 0;
    if (angle_y >= 2*PI)
        angle_y = 0;
    if (angle_z >= 2*PI)
        angle_z = 0;
}

void multiplyMatrixByPoint(const float matrix[3][3], const float point[3], float result[3]) {
    result[0] = (point[0] * matrix[0][0]) + (point[1] * matrix[1][0]) + (point[2] * matrix[2][0]);
    result[1] = (point[0] * matrix[0][1]) + (point[1] * matrix[1][1]) + (point[2] * matrix[2][1]);
    result[2] = (point[0] * matrix[0][2]) + (point[1] * matrix[1][2]) + (point[2] * matrix[2][2]);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    float projection_matrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    };

    // rotation matrices
    float rotation_matrix_z[3][3] = {
        {cos(angle_z), -sin(angle_z), 0},
        {sin(angle_z), cos(angle_z), 0},
        {0, 0, 1},
    };

    float rotation_matrix_y[3][3] = {
        {cos(angle_y), 0, sin(angle_y)},
        {0, 1, 0},
        {-sin(angle_y), 0, cos(angle_y)},
    };

    float rotation_matrix_x[3][3] = {
        {1, 0, 0},
        {0, cos(angle_x), -sin(angle_x)},
        {0, sin(angle_x), cos(angle_x)},
    };

    // rotate and make a projection of each point
    for (int i = 0; i < 8; i++) {
        float* point = points[i];

        // rotation around z axis
        float rotation_z[3];
        multiplyMatrixByPoint(rotation_matrix_z, point, rotation_z);
        
        // rotation around y axis
        float rotation_y[3];
        multiplyMatrixByPoint(rotation_matrix_y, rotation_z, rotation_y);

        // rotation around x axis
        float rotation_x[3];
        multiplyMatrixByPoint(rotation_matrix_x, rotation_y, rotation_x);

        // move cube away from the screen by z_offset
        rotation_x[2] += z_offset;
        
        // divide x and y with z to add perspective 
        float zInv = 1.0f / rotation_x[2];
        rotation_x[0] = rotation_x[0]*zInv;
        rotation_x[1] = -rotation_x[1]*zInv;

        float projection[3];
        multiplyMatrixByPoint(projection_matrix, rotation_x, projection);

        projected_points[i][0] = projection[0];
        projected_points[i][1] = projection[1];
        projected_points[i][2] = projection[2];
    }

    // draw lines based on the edges and points arrays
    for (int i = 0; i < 12; i++) {
        int* edge = edges[i];
        float* point1 = projected_points[edge[0]];
        float* point2 = projected_points[edge[1]];
        
        int x1 = WINDOW_WIDTH/2 + point1[0] * SCALE;
        int y1 = WINDOW_HEIGHT/2 + point1[1] * SCALE;

        int x2 = WINDOW_WIDTH/2 + point2[0] * SCALE;
        int y2 = WINDOW_HEIGHT/2 + point2[1] * SCALE;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    SDL_Color text_color = { 255, 255, 255 }; // White color
    // Render text
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, "SPIN THE CUBE", text_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = { 180, 0, text_surface->w, text_surface->h };
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