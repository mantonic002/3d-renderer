#include "scene.h"

void scene_double_cube_draw(Scene* scene, SDL_Renderer** renderer){
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);
    {
        // rotation matrices for each axis
        Mat3 rotation_matrix_z = mat3_rotation_z(-scene->angle_z);
        Mat3 rotation_matrix_y = mat3_rotation_y(-scene->angle_y);
        Mat3 rotation_matrix_x = mat3_rotation_x(-scene->angle_x);

        // multiply all 3 rotation matrices to get a final rotation matrix
        Mat3 rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
        rotation = multiply_matrices(rotation, rotation_matrix_z);

        // get translation
        Vec3 trans = {0.0f, 0.0f, 5.0f};

        // set pipeline vertex shader
        scene->pipeline->vertex_shader =  create_default_vertex_shader(rotation.data, &trans);

        // render triangles
        pipeline_draw(scene->pipeline, scene->triList);
    }
    {
        // rotation matrices for each axis
        Mat3 rotation_matrix_z = mat3_rotation_z(scene->angle_z);
        Mat3 rotation_matrix_y = mat3_rotation_y(scene->angle_y);
        Mat3 rotation_matrix_x = mat3_rotation_x(scene->angle_x);

        // multiply all 3 rotation matrices to get a final rotation matrix
        Mat3 rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
        rotation = multiply_matrices(rotation, rotation_matrix_z);

        // get translation
        Vec3 trans = {0.0f, 0.0f, scene->z_offset};

        // set pipeline vertex shader
        scene->pipeline->vertex_shader =  create_default_vertex_shader(rotation.data, &trans);

        // render triangles
        pipeline_draw(scene->pipeline, scene->triList);
    }
}