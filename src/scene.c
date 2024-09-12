#include "scene.h"

Scene make_scene(SDL_Renderer** renderer, const char* filename) {
    // make pixel shader
    PixelShader* pixel_shader = create_pixel_shader_texture(filename);
    if (!pixel_shader) {
        fprintf(stderr, "Failed to create pixel shader\n");
        exit(1);
    }

    // make geometry shader
    GeometryShader* geometry_shader = create_default_geometry_shader();
    if (!geometry_shader) {
        fprintf(stderr, "Failed to create geometry shader\n");
        free(pixel_shader);
        exit(1);
    }

    // make pipeline
    Pipeline* pipeline = malloc(sizeof(Pipeline));
    if (!pipeline) {
        fprintf(stderr, "Memory allocation failed for Pipeline\n");
        free(geometry_shader);
        free(pixel_shader);
        exit(1);
    }
    pipeline->renderer = renderer;
    pipeline->geometry_shader = geometry_shader;
    pipeline->pixel_shader = pixel_shader;
    pipeline->vertex_shader = create_default_vertex_shader();
    pipeline->zb = z_buffer_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    Scene scene;
    scene.aspect_ratio = 1.3333f;
    scene.hfov = 100.0f;
    scene.vfov = scene.hfov / scene.aspect_ratio;

    scene.htrack = scene.hfov / WINDOW_WIDTH;
    scene.vtrack = scene.vfov / WINDOW_HEIGHT;
    scene.cam_speed = 0.1f;
    scene.cam_pos = (Vec3){0.0f, 0.0f, 0.0f};
    scene.cam_rot = mat_identity(4);

    scene.mod_pos = (Vec3){0.0f, 0.0f, 3.0f};
    scene.angle_x = 0;
    scene.angle_y = 0;
    scene.angle_z = 0;

    scene.lpos = (Vec3){0.0f, 0.0f, 2.0f};
    scene.time = 0.0f;

    scene.frame_buffer = SDL_CreateRGBSurface(
        0,      
        WINDOW_WIDTH,          
        WINDOW_HEIGHT,              
        32,         // Bits per pixel
        0x00FF0000, // Red mask
        0x0000FF00, // Green mask
        0x000000FF, // Blue mask
        0xFF000000  // Alpha mask
    );
    pipeline->frame_buffer = scene.frame_buffer;

    scene.pipeline = pipeline;

    return scene;
}
