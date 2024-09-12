#include "scene.h"

void scene_point_light_per_pixel_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer and frame buffer
    pipeline_begin_frame(scene->pipeline);

    Mat proj = mat_projection_hfov(scene->hfov, scene->aspect_ratio, 1.0f, 10.0f);
    Mat view_no_rot = mat_translation(-scene->cam_pos.x, -scene->cam_pos.y, -scene->cam_pos.z);
    Mat view = multiply_matrices(view_no_rot, scene->cam_rot);

    // rotation matrices for each axis
    Mat rotation_matrix_z = mat_rotation_z(scene->angle_z, 4);
    Mat rotation_matrix_y = mat_rotation_y(scene->angle_y, 4);
    Mat rotation_matrix_x = mat_rotation_x(scene->angle_x, 4);

    // multiply all 3 rotation matrices to get a final rotation matrix
    Mat rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
    rotation = multiply_matrices(rotation, rotation_matrix_z);

    Mat transformation = multiply_matrices(rotation, mat_translation(scene->mod_pos.x, scene->mod_pos.y, scene->mod_pos.z)); 

    // set pipeline vertex shader
    bind_world(scene->pipeline->vertex_shader, transformation);
    bind_view(scene->pipeline->vertex_shader, view);
    bind_projection(scene->pipeline->vertex_shader, proj);

    // set light position for per pixel shading
    scene->pipeline->pixel_shader->light_pos = multiply_matrix_by_vec3(view, &scene->lpos);
    
    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);

    // set pipeline vertex shader
    Mat light_world_view = multiply_matrices(mat_translation(scene->lpos.x, scene->lpos.y, scene->lpos.z), view);
    bind_world_view(scene->light_pipeline->vertex_shader, light_world_view);
    bind_projection(scene->light_pipeline->vertex_shader, proj);
    
    // render light sphere
    pipeline_draw(scene->light_pipeline, scene->light_sphere);

    // draw contents of frame_buffer
    SDL_Texture* texture = SDL_CreateTextureFromSurface(*renderer, scene->frame_buffer);
    SDL_RenderCopy(*renderer, texture, NULL, NULL);
}

Scene make_scene_point_light_per_pixel(SDL_Renderer** renderer, const char* filename) {
    // make pixel shader
    PixelShader* pixel_shader = create_pixel_shader_per_pixel_lighting(filename);
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
    pipeline->zb = z_buffer_init(WINDOW_WIDTH, WINDOW_HEIGHT);
    pipeline->vertex_shader = create_default_vertex_shader();

    // make pipeline for light point
    Pipeline* light_pipeline = malloc(sizeof(Pipeline));
    if (!light_pipeline) {
        fprintf(stderr, "Memory allocation failed for light pipeline\n");
        free(geometry_shader);
        free(pixel_shader);
        free(pipeline);
        exit(1);
    }
    light_pipeline->renderer = renderer;
    light_pipeline->geometry_shader = geometry_shader;
    light_pipeline->pixel_shader = create_pixel_shader_flat_shade();
    light_pipeline->zb = pipeline->zb;
    light_pipeline->vertex_shader = create_default_vertex_shader();
    
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
    light_pipeline->frame_buffer = scene.frame_buffer;

    scene.pipeline = pipeline;
    scene.light_pipeline = light_pipeline;

    scene.light_sphere = sphere_init_triangle_list(0.05f, 8, 16);
    for (int i = 0; i < scene.light_sphere->sizeV; i++) {
        scene.light_sphere->vertices[i].col = (Vec3){1.0f, 1.0f, 1.0f};
    }

    return scene;
}