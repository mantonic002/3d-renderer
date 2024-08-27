#include "scene.h"

Scene make_scene(SDL_Renderer** renderer, const char* filename) {
    // make pixel shader
    PixelShader* pixel_shader = create_pixel_shader_texture(filename);
    if (!pixel_shader) {
        fprintf(stderr, "Failed to create pixel shader\n");
        exit(1);
    }

    // make pipeline
    Pipeline* pipeline = malloc(sizeof(Pipeline));
    if (!pipeline) {
        fprintf(stderr, "Memory allocation failed for Pipeline\n");
        free(pixel_shader);
        exit(1);
    }
    pipeline->renderer = renderer;
    pipeline->pixel_shader = pixel_shader;
    pipeline->zb = z_buffer_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    // transformation variables
    float angle_x = 0;
    float angle_y = 0;
    float angle_z = 0;
    float z_offset = 5;

    Scene scene = {
        NULL,
        angle_x,
        angle_y,
        angle_z,
        z_offset,
        pipeline,
    };

    return scene;
}
