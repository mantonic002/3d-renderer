#include "scene.h"

void cube_init_triangle_list_independent_faces_w_normals(Scene* scene) {
    // make vertices of a cube
    const int sizeV = 24;
    Vertex* vertices = malloc(sizeV * sizeof(Vertex));
    if (!vertices) {
        fprintf(stderr, "Memory allocation failed for vertices\n");
        exit(1);
    }
    Vec3 temp_vertices[] = {
        {-SIZE, -SIZE, -SIZE},             
        {SIZE, -SIZE, -SIZE},   
        {-SIZE, SIZE, -SIZE},   
        {SIZE, SIZE, -SIZE},    
        {-SIZE, -SIZE, SIZE},   
        {SIZE, -SIZE, SIZE},    
        {-SIZE, SIZE, SIZE},    
        {SIZE, SIZE, SIZE},     
        {-SIZE, -SIZE, -SIZE},  
        {-SIZE, SIZE, -SIZE},   
        {-SIZE, -SIZE, SIZE},   
        {-SIZE, SIZE, SIZE},    
        {SIZE, -SIZE, -SIZE},   
        {SIZE, SIZE, -SIZE},    
        {SIZE, -SIZE, SIZE},    
        {SIZE, SIZE, SIZE},     
        {-SIZE, -SIZE, -SIZE},             
        {SIZE, -SIZE, -SIZE},    
        {-SIZE, -SIZE, SIZE},   
        {SIZE, -SIZE, SIZE},    
        {-SIZE, SIZE, -SIZE},   
        {SIZE, SIZE, -SIZE},    
        {-SIZE, SIZE, SIZE},    
        {SIZE, SIZE, SIZE},     
    };
    for (int i = 0; i < sizeV; i++ ) {
        vertices[i].pos = temp_vertices[i];
    }

    // face normals
    vertices[0].n = (Vec3){ 0.0f,0.0f,-1.0f };
    vertices[1].n = (Vec3){ 0.0f,0.0f,-1.0f };
    vertices[2].n = (Vec3){ 0.0f,0.0f,-1.0f };
    vertices[3].n = (Vec3){ 0.0f,0.0f,-1.0f };
    vertices[4].n = (Vec3){ 0.0f,0.0f,1.0f };
    vertices[5].n = (Vec3){ 0.0f,0.0f,1.0f };
    vertices[6].n = (Vec3){ 0.0f,0.0f,1.0f };
    vertices[7].n = (Vec3){ 0.0f,0.0f,1.0f };
    vertices[8].n = (Vec3){ -1.0f,0.0f,0.0f };
    vertices[9].n = (Vec3){ -1.0f,0.0f,0.0f };
    vertices[10].n = (Vec3){ -1.0f,0.0f,0.0f };
    vertices[11].n = (Vec3){ -1.0f,0.0f,0.0f };
    vertices[12].n = (Vec3){ 1.0f,0.0f,0.0f };
    vertices[13].n = (Vec3){ 1.0f,0.0f,0.0f };
    vertices[14].n = (Vec3){ 1.0f,0.0f,0.0f };
    vertices[15].n = (Vec3){ 1.0f,0.0f,0.0f };
    vertices[16].n = (Vec3){ 0.0f,-1.0f,0.0f };
    vertices[17].n = (Vec3){ 0.0f,-1.0f,0.0f };
    vertices[18].n = (Vec3){ 0.0f,-1.0f,0.0f };
    vertices[19].n = (Vec3){ 0.0f,-1.0f,0.0f };
    vertices[20].n = (Vec3){ 0.0f,1.0f,0.0f };
    vertices[21].n = (Vec3){ 0.0f,1.0f,0.0f };
    vertices[22].n = (Vec3){ 0.0f,1.0f,0.0f };
    vertices[23].n = (Vec3){ 0.0f,1.0f,0.0f };


    // make indices of a cube
    const int sizeI = 12;
    Vec3* indices = malloc(sizeI * sizeof(Vec3));
    if (!indices) {
        fprintf(stderr, "Memory allocation failed for indices\n");
        free(vertices);
        exit(1);
    }
    Vec3 temp_indices[] = {
        {0,2, 1},   {2,3,1},
        {4,5, 7},   {4,7,6},
        {8,10, 9},  {10,11,9},
        {12,13,15}, {12,15,14},
        {16,17,18}, {18,17,19},
        {20,23,21}, {20,22,23}
    };
    memcpy(indices, temp_indices, sizeI * sizeof(Vec3));

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = sizeV;
    triList->indices = indices;
    triList->sizeI = sizeI;

    scene->triList = triList;
}

void scene_flat_shade_draw(Scene* scene, SDL_Renderer** renderer) {
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

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
    scene->pipeline->vertex_shader =  create_flat_shading_vertex_shader(rotation.data, &trans);

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

    Scene scene;
    scene.angle_x = 0;
    scene.angle_y = 0;
    scene.angle_z = 0;
    scene.z_offset = 5;
    scene.time = 0.0f,
    scene.pipeline = pipeline;
    
    return scene;
}