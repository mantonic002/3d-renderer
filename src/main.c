#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "./constants.h"

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void restart_cube(void);


Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void get_pixel_rgba(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;
Vec3 cross_product (const Vec3* v1, const Vec3* v2);
float dot_product (const Vec3* v1, const Vec3* v2);
Vec3 vec3_subtract (const Vec3* v1, const Vec3* v2);
Vec3 vec3_interpolate (const Vec3* v1, const Vec3* v2, const float alpha);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Vec2 vec2_interpolate (const Vec2* v1, const Vec2* v2, const float alpha);
Vec2 vec2_subtract (const Vec2* v1, const Vec2* v2);
Vec2 vec2_add (const Vec2* v1, const Vec2* v2);
Vec2 vec2_divide (const Vec2* v, float scalar);
Vec2 vec2_multiply (const Vec2* v, float scalar);



void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);
void draw_flat_top_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);
void draw_flat_bottom_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c);

typedef struct TexVertex
{
    Vec2 pos; // position
    Vec2 tc; // texture coordinate
} TexVertex;

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha);

void draw_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s);
void draw_flat_top_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s);
void draw_flat_bottom_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s);

bool game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
float delta_time = 0.0f;

bool keys[SDL_NUM_SCANCODES] = { false };

TTF_Font* font = NULL;
SDL_Surface* textureFromPng = NULL;

bool wireframe = false;
bool textured = false;

// vertices of a cube
Vec3 vertices[8] = {
    {-SIZE, -SIZE, -SIZE},
    {SIZE, -SIZE, -SIZE},
    {-SIZE, SIZE, -SIZE},
    {SIZE, SIZE, -SIZE}, 
    {-SIZE, -SIZE, SIZE},
    {SIZE, -SIZE, SIZE}, 
    {-SIZE, SIZE, SIZE}, 
    {SIZE, SIZE, SIZE},  
};

TexVertex projected_points[8] = {
    {{-SIZE, -SIZE}, {0.0f, 1.0f}},
    {{SIZE, -SIZE},  {1.0f, 1.0f}},
    {{-SIZE, SIZE},  {0.0f, 0.0f}},
    {{SIZE, SIZE},   {1.0f, 0.0f}},
    {{-SIZE, -SIZE},  {1.0f, 1.0f}},
    {{SIZE, -SIZE},   {0.0f, 1.0f}},
    {{-SIZE, SIZE},   {1.0f, 0.0f}},
    {{SIZE, SIZE},    {0.0f, 0.0f}},
};

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
    {7, 4, 5}, {4, 7, 6},
    {0, 4, 2}, {2, 4, 6},
    {0, 1, 4}, {1, 5, 4},
};

// TexVertex tex_indices[12] = {


// };

// cull flag for each triangle to check if it's being drawn
bool cullFlags[12] = {false};

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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Error initializing SDL_image.\n");
        return false;
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

    textureFromPng = IMG_Load("src/texture.png");
    if (!textureFromPng) {
        fprintf(stderr, "Unable to load image! SDL_Error: %s\n", SDL_GetError());
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
            if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                wireframe = !wireframe;
                textured = false;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_T) {
                textured = !textured;
                wireframe = false;
            }

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
        z_offset += 0.5f;
    }

    if (keys[SDL_SCANCODE_DOWN] && z_offset > 2.1f && z_offset <= 20)
    {
        z_offset -= 0.5f;
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

    float zInv;

    //TODO: Everything in this loops keeps repeating for some reason
    // rotate and make a projection of each point
    for (int i = 0; i < 8; i++) {
        Vec3 point = vertices[i];

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

        vertices[i] = rotation_x;
    }

    // backface culling
    for (int i = 0; i < 12; i++) {
        Vec3 v1 = vertices[indices[i][0]];
        Vec3 v2 = vertices[indices[i][1]];
        Vec3 v3 = vertices[indices[i][2]];

        Vec3 v2_sub_v1 = vec3_subtract(&v2, &v1);
        Vec3 v3_sub_v1 = vec3_subtract(&v3, &v1);
        Vec3 n = cross_product(&v2_sub_v1, &v3_sub_v1);
        cullFlags[i] =  dot_product(&n, &v1) > 0.0f;
    }

    // projection
    for (int i = 0; i < 8; i++) {
        Vec3 curr = vertices[i];
        
        zInv = 1.0f / curr.z;
        curr.x = curr.x*zInv;
        curr.y = -curr.y*zInv;

        Vec3 projection;
        multiplyMatrixByPoint(projection_matrix, &curr, &projection);

        projected_points[i].pos.x = WINDOW_WIDTH/2 + curr.x * SCALE;
        projected_points[i].pos.y = WINDOW_HEIGHT/2 + curr.y * SCALE;
    }

    restart_cube();

    // draw lines based on the edges and vertices arrays
    if (wireframe) {
        for (int i = 0; i < 12; i++) {
            int* edge = edges[i];
            Vec2 p1 = projected_points[edge[0]].pos;
            Vec2 p2 = projected_points[edge[1]].pos;

            SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }
    }
    else if (textured) {
        for (int i = 0; i < 12; i++) {
            if (!cullFlags[i]) {                
                int* index = indices[i];
                TexVertex tv1 = projected_points[index[0]];
                TexVertex tv2 = projected_points[index[1]];
                TexVertex tv3 = projected_points[index[2]];

                draw_triangle_tex(&tv1, &tv2, &tv3, textureFromPng);
            }
        }
    }
    else {
        SDL_Color colors[12] = {
            { 125, 125, 125, 255 },
            { 255, 0, 0, 255 },
            { 0, 0, 255, 255 },
            { 100, 25, 124, 255 },
            { 255, 255, 255, 255 },
            { 0, 255, 255, 255 },
            { 255, 15, 0, 100 },
            { 255, 255, 0, 255 },
            { 124, 124, 0, 255 },
            { 255, 2, 255, 255 },
            { 0, 124, 124, 255 },
            { 255, 255, 255, 125 },
        };

        // draw triangles
        for (int i = 0; i < 12; i++) {
            if (!cullFlags[i]) {                
                int* index = indices[i];
                Vec2 v1 = projected_points[index[0]].pos;
                Vec2 v2 = projected_points[index[1]].pos;
                Vec2 v3 = projected_points[index[2]].pos;

                draw_triangle(&v1, &v2, &v3, colors[i]);
            }
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
void ptr_swap(void* p1, void* p2, size_t size) {
    void* temp = malloc(size);
    if (temp == NULL) {
        return;
    }
    
    memcpy(temp, p1, size);
    memcpy(p1, p2, size);  
    memcpy(p2, temp, size);
    
    free(temp);
}

void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c) {
    // sort vertices by y (height)
    if (v2->y < v1->y) 
        ptr_swap(v1, v2, sizeof(Vec2));
    if (v3->y < v2->y) 
        ptr_swap(v2, v3, sizeof(Vec2));
    if (v2->y < v1->y) 
        ptr_swap(v1, v2, sizeof(Vec2));

    if (v1->y == v2->y) { // natural flat top
        if (v2->x < v1->x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(Vec2));
        draw_flat_top_triangle(v1, v2, v3, c);
    }
    else if (v2->y == v3->y) { // natural flat bottom
        if (v3->x < v2->x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(Vec2));
        draw_flat_bottom_triangle(v1, v2, v3, c);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->y - v1->y) / (v3->y - v1->y);
        // Interpolate to find the point between v3 and v1
        Vec2 vi = vec2_interpolate(v1, v3, alpha);

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

void draw_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s) {
    // sort vertices by y (height)
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(TexVertex));
    if (v3->pos.y < v2->pos.y) 
        ptr_swap(v2, v3, sizeof(TexVertex));
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(TexVertex));

    if (v1->pos.y == v2->pos.y) { // natural flat top
        if (v2->pos.x < v1->pos.x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(TexVertex));
        draw_flat_top_triangle_tex(v1, v2, v3, s);
    }
    else if (v2->pos.y == v3->pos.y) { // natural flat bottom
        if (v3->pos.x < v2->pos.x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(TexVertex));
        draw_flat_bottom_triangle_tex(v1, v2, v3, s);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
        // Interpolate to find the point between v3 and v1
        TexVertex vi = tex_vertex_interpolate(v1, v3, alpha);

        if (v2->pos.x < vi.pos.x) { // major right
            draw_flat_bottom_triangle_tex(v1, v2, &vi, s);
            draw_flat_top_triangle_tex(v2, &vi, v3, s);
        }
        else { // major left
            draw_flat_bottom_triangle_tex(v1, &vi, v2, s);
            draw_flat_top_triangle_tex(&vi, v2, v3, s);
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
        // calc Xs for start and end vertices
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

void draw_flat_top_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v3->pos.x - v1->pos.x) / (v3->pos.y - v1->pos.y);
    float m2 = (v3->pos.x - v2->pos.x) / (v3->pos.y - v2->pos.y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.y - 0.5f); // not drawn

    // texture coordinate points
    Vec2 tcEdgeL = v1->tc;
    Vec2 tcEdgeR = v2->tc;
    Vec2 tcBottom = v3->tc;

    // texture coordinate edge unit step
    Vec2 b_sub_l = vec2_subtract(&tcBottom, &tcEdgeL);
    const Vec2 tcEdgeStepL = vec2_divide(&b_sub_l, (v3->pos.y - v1->pos.y));

    Vec2 b_sub_r = vec2_subtract(&tcBottom, &tcEdgeR);
    const Vec2 tcEdgeStepR = vec2_divide(&b_sub_r, (v3->pos.y - v2->pos.y));

    // add texture edge prestep
    Vec2 prestep = vec2_multiply(&tcEdgeStepL, ((float)yStart + 0.5f - v2->pos.y));
    tcEdgeL = vec2_add(&tcEdgeL, &prestep);

    prestep = vec2_multiply(&tcEdgeStepR, ((float)yStart + 0.5f - v2->pos.y));
    tcEdgeR = vec2_add(&tcEdgeR, &prestep);

    // init texture w/h and clamp values
    const float tex_width = (float)s->w;
    const float tex_height = (float)s->h;
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    for (int y = yStart; y < yEnd; y++,
            tcEdgeL = vec2_add(&tcEdgeL, &tcEdgeStepL),
            tcEdgeR = vec2_add(&tcEdgeR, &tcEdgeStepR)) 
    {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;
        float x2 = m2 * ((float)y + 0.5f - v2->pos.y) + v2->pos.x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        // texture coordinate scanline unit step
        Vec2 temp = vec2_subtract(&tcEdgeR, &tcEdgeL);
        const Vec2 tcScanStep = vec2_divide(&temp, (x2 - x1));

        temp = vec2_add(&tcEdgeL, &tcScanStep);
        Vec2 tc = vec2_multiply(&temp, ((float)xStart + 0.5f - x1));

        for (int x = xStart; x < xEnd; x++, tc = vec2_add(&tc, &tcScanStep)) {
            Uint8 r, g, b, a;
            get_pixel_rgba( s, 
                            fmin(tc.x * tex_width, tex_clamp_x),
                            fmin(tc.y * tex_height, tex_clamp_y),
                            &r, &g, &b, &a);

            SDL_SetRenderDrawColor(renderer, r, g, b, a);
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
        // calc Xs for start and end vertices
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

void draw_flat_bottom_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v2->pos.x - v1->pos.x) / (v2->pos.y - v1->pos.y);
    float m2 = (v3->pos.x - v1->pos.x) / (v3->pos.y - v1->pos.y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.y - 0.5f); // not drawn

    // texture coordinate points
    Vec2 tcEdgeL = v1->tc;
    Vec2 tcEdgeR = v1->tc;
    Vec2 tcBottomL = v2->tc;
    Vec2 tcBottomR = v3->tc;

    // texture coordinate edge unit step
    Vec2 b_sub_l = vec2_subtract(&tcBottomL, &tcEdgeL);
    const Vec2 tcEdgeStepL = vec2_divide(&b_sub_l, (v2->pos.y - v1->pos.y));

    Vec2 b_sub_r = vec2_subtract(&tcBottomR, &tcEdgeR);
    const Vec2 tcEdgeStepR = vec2_divide(&b_sub_r, (v3->pos.y - v1->pos.y));

    // add texture edge prestep
    Vec2 prestep = vec2_multiply(&tcEdgeStepL, ((float)yStart + 0.5f - v1->pos.y));
    tcEdgeL = vec2_add(&tcEdgeL, &prestep);

    prestep = vec2_multiply(&tcEdgeStepR, ((float)yStart + 0.5f - v1->pos.y));
    tcEdgeR = vec2_add(&tcEdgeR, &prestep);

    // init texture w/h and clamp values
    const float tex_width = (float)s->w;
    const float tex_height = (float)s->h;
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    for (int y = yStart; y < yEnd; y++,
            tcEdgeL = vec2_add(&tcEdgeL, &tcEdgeStepL),
            tcEdgeR = vec2_add(&tcEdgeR, &tcEdgeStepR)) 
    {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;
        float x2 = m2 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        // texture coordinate scanline unit step
        Vec2 temp = vec2_subtract(&tcEdgeR, &tcEdgeL);
        const Vec2 tcScanStep = vec2_divide(&temp, (x2 - x1));

        temp = vec2_add(&tcEdgeL, &tcScanStep);
        Vec2 tc = vec2_multiply(&temp, ((float)xStart + 0.5f - x1));

        for (int x = xStart; x < xEnd; x++, tc = vec2_add(&tc, &tcScanStep)) {
            Uint8 r, g, b, a;
            get_pixel_rgba( s, 
                            fmin(tc.x * tex_width, tex_clamp_x),
                            fmin(tc.y * tex_height, tex_clamp_y),
                            &r, &g, &b, &a);

            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

Vec3 cross_product (const Vec3* v1, const Vec3* v2) {
    Vec3 result = { 
        (v1->y * v2->z) - (v1->z * v2->y), 
        (v1->z * v2->x) - (v1->x * v2->z), 
        (v1->x * v2->y) - (v1->y * v2->x), 
    };
    return result;
}

float dot_product (const Vec3* v1, const Vec3* v2) {
    float result = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z; 
    return result;
}

Vec3 vec3_subtract (const Vec3* v1, const Vec3* v2) {
    Vec3 result = { 
        v1->x - v2->x,
        v1->y - v2->y,
        v1->z - v2->z,
    };
    return result;
}


Vec3 vec3_interpolate (const Vec3* v1, const Vec3* v2, const float alpha) {
    Vec3 result = {
        v1->x + (v2->x - v1->x) * alpha,
        v1->y + (v2->y - v1->y) * alpha,
        v1->z + (v2->z - v1->z) * alpha,
    };
    return result;
}

Vec2 vec2_interpolate (const Vec2* v1, const Vec2* v2, const float alpha) {
    Vec2 result = {
        v1->x + (v2->x - v1->x) * alpha,
        v1->y + (v2->y - v1->y) * alpha,
    };
    return result;
}

Vec2 vec2_subtract (const Vec2* v1, const Vec2* v2) {
    Vec2 result = { 
        v1->x - v2->x,
        v1->y - v2->y,
    };
    return result;
}

Vec2 vec2_add (const Vec2* v1, const Vec2* v2) {
    Vec2 result = { 
        v1->x + v2->x,
        v1->y + v2->y,
    };
    return result;
}

Vec2 vec2_divide (const Vec2* v, float scalar) {
    Vec2 result = { 
        v->x/scalar,
        v->y/scalar,
    };
    return result;
}

Vec2 vec2_multiply (const Vec2* v, float scalar) {
    Vec2 result = { 
        v->x*scalar,
        v->y*scalar,
    };
    return result;
}

TexVertex tex_vertex_interpolate (const TexVertex* v1, const TexVertex* v2, const float alpha) {
    TexVertex result = {
        vec2_interpolate(&v1->pos, &v2->pos, alpha),
        vec2_interpolate(&v1->tc, &v2->tc, alpha),
    };
    return result;
}


void restart_cube() {
    vertices[0].x = -SIZE; vertices[0].y = -SIZE; vertices[0].z = -SIZE;
    vertices[1].x = SIZE;  vertices[1].y = -SIZE; vertices[1].z = -SIZE;
    vertices[2].x = -SIZE; vertices[2].y = SIZE;  vertices[2].z = -SIZE;
    vertices[3].x = SIZE;  vertices[3].y = SIZE;  vertices[3].z = -SIZE;
    vertices[4].x = -SIZE; vertices[4].y = -SIZE; vertices[4].z = SIZE;
    vertices[5].x = SIZE;  vertices[5].y = -SIZE; vertices[5].z = SIZE;
    vertices[6].x = -SIZE; vertices[6].y = SIZE;  vertices[6].z = SIZE;
    vertices[7].x = SIZE;  vertices[7].y = SIZE;  vertices[7].z = SIZE;
}

Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
    // check if coordinates are within the bounds of the surface
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        return 0; 
    }

    // pixel's position in the surface's pixel buffer
    int bpp = surface->format->BytesPerPixel;
    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

    Uint32 pixel_value;
    memcpy(&pixel_value, pixel_ptr, bpp);

    return pixel_value;
}

void get_pixel_rgba(SDL_Surface* surface, int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a) {
    Uint32 pixel_value = get_pixel(surface, x, y);
    SDL_GetRGBA(pixel_value, surface->format, r, g, b, a);
}


void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}