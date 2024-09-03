#include "scene.h"

void model_init_triangle_list(Scene* scene, const char *filename) {
    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        exit(1);
    }
    load_obj(filename, triList);

    scene->triList = triList;

    scene->light_sphere = sphere_init_triangle_list(0.2f, 8, 16);
    for (int i = 0; i < scene->light_sphere->sizeV; i++) {
        scene->light_sphere->vertices[i].col = (Vec3){1.0f, 1.0f, 1.0f};
    }
}

void scene_model_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

    // rotation matrices for each axis
    Mat3 rotation_matrix_z = mat3_rotation_z(scene->angle_z);
    Mat3 rotation_matrix_y = mat3_rotation_y(scene->angle_y);
    Mat3 rotation_matrix_x = mat3_rotation_x(scene->angle_x);

    // multiply all 3 rotation matrices to get a final rotation matrix
    Mat3 rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
    rotation = multiply_matrices(rotation, rotation_matrix_z);

    // set pipeline vertex shader
    scene->pipeline->vertex_shader->light_pos = scene->lpos;
    memcpy(scene->pipeline->vertex_shader->rotation, &rotation, 3 * 3 * sizeof(float));
    scene->pipeline->vertex_shader->translation = (Vec3){0.0f, 0.0f, scene->z_offset};

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);

    // set pipeline vertex shader
    scene->light_pipeline->vertex_shader->light_pos = scene->lpos;
    memcpy(scene->light_pipeline->vertex_shader->rotation, &rotation, 3 * 3 * sizeof(float));
    scene->light_pipeline->vertex_shader->translation = scene->lpos;
    
    // render light sphere
    pipeline_draw(scene->light_pipeline, scene->light_sphere);
}

Scene make_scene_model(SDL_Renderer** renderer, const char* filename) {
    // make pixel shader
    PixelShader* pixel_shader;
    if (filename[0] == '\0') {
        pixel_shader = create_pixel_shader_flat_shade();
    } else {
        pixel_shader = create_pixel_shader_flat_tex(filename);
    }
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
    pipeline->vertex_shader = create_point_light_vertex_shader();

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
    VertexShader* vs = create_default_vertex_shader();
    if (!vs) {
        fprintf(stderr, "Memory allocation failed for vs\n");
        free(geometry_shader);
        free(pixel_shader);
        free(pipeline);
        exit(1);
    }
    light_pipeline->vertex_shader = vs;
    
    Scene scene;
    scene.angle_x = 0;
    scene.angle_y = 0;
    scene.angle_z = 0;
    scene.z_offset = 5;
    scene.time = 0.0f,
    scene.lpos = (Vec3){0.0f, 0.0f, 1.0f};
    scene.pipeline = pipeline;
    scene.light_pipeline = light_pipeline;

    return scene;
}