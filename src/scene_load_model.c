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
}

void scene_model_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

    // rotation matrices for each axis
    float rotation_matrix_z[3][3] = {
        {cos(scene->angle_z), -sin(scene->angle_z), 0},
        {sin(scene->angle_z), cos(scene->angle_z), 0},
        {0, 0, 1},
    };

    float rotation_matrix_y[3][3] = {
        {cos(scene->angle_y), 0, sin(scene->angle_y)},
        {0, 1, 0},
        {-sin(scene->angle_y), 0, cos(scene->angle_y)},
    };

    float rotation_matrix_x[3][3] = {
        {1, 0, 0},
        {0, cos(scene->angle_x), -sin(scene->angle_x)},
        {0, sin(scene->angle_x), cos(scene->angle_x)},
    };

    // multiply all 3 rotation matrices to get a final rotation matrix
    float temp[3][3];
    multiply_matrices(rotation_matrix_x, rotation_matrix_y, temp);
    float rotation[3][3];
    multiply_matrices(temp, rotation_matrix_z, rotation);

    // get translation
    Vec3 trans = {0.0f, 0.0f, scene->z_offset};

    // set pipeline vertex shader
    scene->pipeline->vertex_shader =  create_flat_shading_vertex_shader(rotation, &trans);

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}

Scene make_scene_model(SDL_Renderer** renderer, const char* filename) {
    // make pixel shader
    PixelShader* pixel_shader = create_pixel_shader_flat_tex(filename);
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
        0.0f,
        pipeline,
    };

    return scene;
}