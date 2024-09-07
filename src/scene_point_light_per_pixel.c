#include "scene.h"

void scene_point_light_per_pixel_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

    Mat proj = mat_projection_hfov(100.0f, 1.33333f, 1.0f, 10.0f);

    // rotation matrices for each axis
    Mat rotation_matrix_z = mat_rotation_z(scene->angle_z, 4);
    Mat rotation_matrix_y = mat_rotation_y(scene->angle_y, 4);
    Mat rotation_matrix_x = mat_rotation_x(scene->angle_x, 4);

    // multiply all 3 rotation matrices to get a final rotation matrix
    Mat rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
    rotation = multiply_matrices(rotation, rotation_matrix_z);

    Mat transformation = multiply_matrices(rotation, mat_translation(0.0f, 0.0f, scene->z_offset)); 

    // set pipeline vertex shader
    bind_projection(scene->pipeline->vertex_shader, proj);
    bind_world(scene->pipeline->vertex_shader, transformation);

    // set light position for per pixel shading
    scene->pipeline->pixel_shader->light_pos = scene->lpos;

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);

    // set pipeline vertex shader
    bind_projection(scene->light_pipeline->vertex_shader, proj);
    bind_world(scene->light_pipeline->vertex_shader, mat_translation(scene->lpos.x, scene->lpos.y, scene->lpos.z));

    // render light sphere
    pipeline_draw(scene->light_pipeline, scene->light_sphere);
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
    scene.angle_x = 0;
    scene.angle_y = 0;
    scene.angle_z = 0;
    scene.z_offset = 3;
    scene.time = 0.0f,
    scene.lpos = (Vec3){0.0f, 0.0f, 2.5f};
    scene.pipeline = pipeline;
    scene.light_pipeline = light_pipeline;

    scene.light_sphere = sphere_init_triangle_list(0.05f, 8, 16);
    for (int i = 0; i < scene.light_sphere->sizeV; i++) {
        scene.light_sphere->vertices[i].col = (Vec3){1.0f, 1.0f, 1.0f};
    }

    return scene;
}