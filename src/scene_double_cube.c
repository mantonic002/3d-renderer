// #include "scene.h"

// void scene_double_cube_draw(Scene* scene, SDL_Renderer** renderer){
//     // clear z buffer
//     pipeline_begin_frame(scene->pipeline);
//     {
//         // rotation matrices for each axis
//         Mat rotation_matrix_z = mat_rotation_z(-scene->angle_z, 3);
//         Mat rotation_matrix_y = mat_rotation_y(-scene->angle_y, 3);
//         Mat rotation_matrix_x = mat_rotation_x(-scene->angle_x, 3);

//         // multiply all 3 rotation matrices to get a final rotation matrix
//         Mat rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
//         rotation = multiply_matrices(rotation, rotation_matrix_z);

//         // set pipeline vertex shader
//         memcpy(&scene->pipeline->vertex_shader->rotation, &rotation, sizeof(Mat));
//         scene->pipeline->vertex_shader->translation = (Vec3){0.0f, 0.0f, 5.0f};

//         // render triangles
//         pipeline_draw(scene->pipeline, scene->triList);
//     }
//     {
//         // rotation matrices for each axis
//         Mat rotation_matrix_z = mat_rotation_z(scene->angle_z, 3);
//         Mat rotation_matrix_y = mat_rotation_y(scene->angle_y, 3);
//         Mat rotation_matrix_x = mat_rotation_x(scene->angle_x, 3);

//         // multiply all 3 rotation matrices to get a final rotation matrix
//         Mat rotation = multiply_matrices(rotation_matrix_x, rotation_matrix_y);
//         rotation = multiply_matrices(rotation, rotation_matrix_z);

//         // set pipeline vertex shader
//         memcpy(&scene->pipeline->vertex_shader->rotation, &rotation, sizeof(Mat));
//         scene->pipeline->vertex_shader->translation = (Vec3){0.0f, 0.0f, scene->z_offset};

//         // render triangles
//         pipeline_draw(scene->pipeline, scene->triList);
//     }
// }