#include "scene.h"

void scene_double_cube_draw(Scene* scene, SDL_Renderer** renderer){
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);
    {
        // rotation matrices for each axis
        float rotation_matrix_z[3][3] = {
            {cos(-scene->angle_z), -sin(-scene->angle_z), 0},
            {sin(-scene->angle_z), cos(-scene->angle_z), 0},
            {0, 0, 1},
        };

        float rotation_matrix_y[3][3] = {
            {cos(-scene->angle_y), 0, sin(-scene->angle_y)},
            {0, 1, 0},
            {-sin(-scene->angle_y), 0, cos(-scene->angle_y)},
        };

        float rotation_matrix_x[3][3] = {
            {1, 0, 0},
            {0, cos(-scene->angle_x), -sin(-scene->angle_x)},
            {0, sin(-scene->angle_x), cos(-scene->angle_x)},
        };

        // multiply all 3 rotation matrices to get a final rotation matrix
        float temp[3][3];
        multiply_matrices(rotation_matrix_x, rotation_matrix_y, temp);
        float rotation[3][3];
        multiply_matrices(temp, rotation_matrix_z, rotation);

        // get translation
        Vec3 trans = {0.0f, 0.0f, 5.0f};

        // set pipeline vertex shader
        scene->pipeline->vertex_shader =  create_default_vertex_shader(rotation, &trans);

        // render triangles
        pipeline_draw(scene->pipeline, scene->triList);
    }
    {
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
        scene->pipeline->vertex_shader =  create_default_vertex_shader(rotation, &trans);

        // render triangles
        pipeline_draw(scene->pipeline, scene->triList);
    }
}