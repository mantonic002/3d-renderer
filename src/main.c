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

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);
void draw_flat_top_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);
void draw_flat_bottom_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);

bool game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
float delta_time = 0.0f;

bool keys[SDL_NUM_SCANCODES] = { false };

TTF_Font* font = NULL;

// points of a cube
Vec3 points[8] = {
    {-SIZE, -SIZE, -SIZE},
    {SIZE, -SIZE, -SIZE},
    {-SIZE, SIZE, -SIZE},
    {SIZE, SIZE, -SIZE},
    {-SIZE, -SIZE, SIZE},
    {SIZE, -SIZE, SIZE},
    {-SIZE, SIZE, SIZE},
    {SIZE, SIZE, SIZE},
};

Vec2 projected_points[8] = {0};

// indexed line list
int edges[12][2] = {
    {0, 1}, {1, 3}, 
    {3, 2}, {2, 0}, 
    {0, 4}, {1, 5}, 
    {3, 7}, {2, 6}, 
    {4, 5}, {5, 7}, 
    {7, 6}, {6, 4}, 
};

// indexed triangle list
int indices[12][3] = {
    {0, 2, 1}, {2, 3, 1},
    {1, 3, 5}, {3, 7, 5},
    {2, 6, 3}, {3, 6, 7},
    {4, 7, 5}, {4, 7, 6},
    {0, 4, 2}, {2, 4, 6},
    {0, 1, 4}, {1, 5, 4},
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

void multiplyMatrixByPoint(const float matrix[3][3], const Vec3* point, Vec3* result) {
    result->x = (point->x * matrix[0][0]) + (point->y * matrix[1][0]) + (point->z * matrix[2][0]);
    result->y = (point->x * matrix[0][1]) + (point->y * matrix[1][1]) + (point->z * matrix[2][1]);
    result->z = (point->x * matrix[0][2]) + (point->y * matrix[1][2]) + (point->z * matrix[2][2]);
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
        Vec3 point = points[i];

        // rotation around z axis
        Vec3 rotation_z;
        multiplyMatrixByPoint(rotation_matrix_z, &point, &rotation_z);
        
        // rotation around y axis
        Vec3 rotation_y;
        multiplyMatrixByPoint(rotation_matrix_y, &rotation_z, &rotation_y);

        // rotation around x axis
        Vec3 rotation_x;
        multiplyMatrixByPoint(rotation_matrix_x, &rotation_y, &rotation_x);

        // move cube away from the screen by z_offset
        rotation_x.z += z_offset;
        
        // divide x and y with z to add perspective 
        float zInv = 1.0f / rotation_x.z;
        rotation_x.x = rotation_x.x*zInv;
        rotation_x.y = -rotation_x.y*zInv;

        Vec3 projection;
        multiplyMatrixByPoint(projection_matrix, &rotation_x, &projection);

        projected_points[i].x = WINDOW_WIDTH/2 + projection.x * SCALE;
        projected_points[i].y = WINDOW_HEIGHT/2 + projection.y * SCALE;
    }

    bool wireframe = false;;
    // draw lines based on the edges and points arrays
    if (wireframe) {
        for (int i = 0; i < 12; i++) {
            int* edge = edges[i];
            Vec2 p1 = projected_points[edge[0]];
            Vec2 p2 = projected_points[edge[1]];

            SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }
    }
    else {
        SDL_Color colors[12] = {
           { 255, 255, 255, 255 },
           { 0, 255, 255, 255 },
           { 255, 0, 255, 255 },
           { 255, 255, 0, 255 },
           { 125, 125, 125, 255 },
           { 255, 0, 0, 255 },
           { 0, 0, 255, 255 },
           { 100, 25, 124, 255 },
           { 124, 124, 0, 255 },
           { 255, 0, 255, 255 },
           { 0, 124, 124, 255 },
           { 255, 255, 255, 125 },
        };
        
        for (int i = 0; i < 12; i++) {
            int* index = indices[i];
            Vec2 v1 = projected_points[index[0]];
            Vec2 v2 = projected_points[index[1]];
            Vec2 v3 = projected_points[index[2]];
            

            draw_triangle(&v1, &v2, &v3, colors[i]);
        }
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

// helper func for swapping pointers
void ptr_swap(Vec2* p1, Vec2* p2) {
    Vec2 temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c) {
    // sort vertices by y (height)
    if (v2->y < v1->y) 
        ptr_swap(v1, v2);
    if (v3->y < v2->y) 
        ptr_swap(v2, v3);
    if (v2->y < v1->y) 
        ptr_swap(v1, v2);

    if (v1->y == v2->y) { // natural flat top
        if (v2->x < v1->x) // sort top vertices by x
            ptr_swap(v1, v2);
        draw_flat_top_triangle(v1, v2, v3, c);
    }
    else if (v2->y == v3->y) { // natural flat bottom
        if (v3->x < v2->x) // sort bottom vertices by x
            ptr_swap(v2, v3);
        draw_flat_bottom_triangle(v1, v2, v3, c);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex
        float split = (v2->y - v1->y) / (v3->y - v1->y);
        Vec2 vi;
        vi.x =  v1->x + (v3->x - v1->x) * split;
        vi.y =  v1->y + (v3->y - v1->y) * split;

        if (v2->x < vi.x) { // major right
            draw_flat_bottom_triangle(v1, v2, &vi, c);
            draw_flat_top_triangle(v2, &vi, v3, c);
        }
        else { // major left
            draw_flat_bottom_triangle(v1, &vi, v2, c);
            draw_flat_top_triangle(&vi, v2, v3, c);
        }
    }
}

void draw_flat_top_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v3->x - v1->x) / (v3->y - v1->y);
    float m2 = (v3->x - v2->x) / (v3->y - v2->y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->y - 0.5f);
    int yEnd = (int)ceil( v3->y - 0.5f); // not drawn

    for (int y = yStart; y < yEnd; y++) {
        // calc Xs for start and end points
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->y) + v1->x;
        float x2 = m2 * ((float)y + 0.5f - v2->y) + v2->x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        for (int x = xStart; x < xEnd; x++) {
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void draw_flat_bottom_triangle  (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v2->x - v1->x) / (v2->y - v1->y);
    float m2 = (v3->x - v1->x) / (v3->y - v1->y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->y - 0.5f);
    int yEnd = (int)ceil( v3->y - 0.5f); // not drawn

    for (int y = yStart; y < yEnd; y++) {
        // calc Xs for start and end points
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->y) + v1->x;
        float x2 = m2 * ((float)y + 0.5f - v1->y) + v1->x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        for (int x = xStart; x < xEnd; x++) {
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
//     printf("X--v1: %f, v2: %f, v3: %f\n", v1->x, v2->x, v3->x);
// printf("Y--v1: %f, v2: %f, v3: %f\n", v1->y, v2->y, v3->y);