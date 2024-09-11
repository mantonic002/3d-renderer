#include "scene.h"

void scene_flat_shade_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
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

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}

Scene make_scene_flat_shade(SDL_Renderer** renderer) {
    // make pixel shader
    PixelShader* pixel_shader = create_pixel_shader_flat_shade();
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
    pipeline->vertex_shader =  create_flat_shading_vertex_shader();

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
    scene.pipeline = pipeline;
    
    return scene;
}