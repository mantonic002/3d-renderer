#include "scene.h"

int compute_indices(int x, int y, int nVerticesSide) {
    return y * nVerticesSide + x;
}

void plane_init_triangle_list(Scene* scene, int divisions, float size) {
    int nVerticesSide = divisions + 1;
    int numVertices = SQUARE(nVerticesSide);
    int numIndices = SQUARE(divisions) * 2;

    // Allocate memory for vertices and indices
    Vertex* vertices = (Vertex *)malloc(numVertices * sizeof(Vertex));
    Vec3* indices = (Vec3 *)malloc(numIndices * sizeof(Vec3));

    if (!vertices || !indices) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed for indices or indices\n");
        exit(1);
    }

    float side = size / 2.0f;
    float divisionSize = size / (float)divisions;
    Vec3 bottomLeft = {-side, -side, 0.0f};

    // Compute vertex positions
    for (int y = 0, i = 0; y < nVerticesSide; y++) {
        float y_pos = (float)y * divisionSize;
        for (int x = 0; x < nVerticesSide; x++, i++) {
            vertices[i].pos.as_vec3 = (Vec3){
                bottomLeft.x + (float)x * divisionSize,
                bottomLeft.y + y_pos,
                0.0f
            };
        }
    }

    // Compute indices
    int index = 0;
    for (int y = 0; y < divisions; y++) {
        for (int x = 0; x < divisions; x++) {
            int indexArray[4] = {
                compute_indices(x, y, nVerticesSide),
                compute_indices(x + 1, y, nVerticesSide),
                compute_indices(x, y + 1, nVerticesSide),
                compute_indices(x + 1, y + 1, nVerticesSide)
            };

            indices[index].x = indexArray[0];
            indices[index].y = indexArray[2];
            indices[index++].z = indexArray[1];
            indices[index].x = indexArray[1];
            indices[index].y = indexArray[2];
            indices[index++].z = indexArray[3];
        }
    }

    // make triList
    IndexedTriangleList* triList = malloc(sizeof(IndexedTriangleList));
    if (!triList) {
        fprintf(stderr, "Memory allocation failed for triList\n");
        free(vertices);
        free(indices);
        exit(1);
    }
    triList->vertices = vertices;
    triList->sizeV = numVertices;
    triList->indices = indices;
    triList->sizeI = numIndices;

    scene->triList = triList;
}

void plane_init_triangle_list_skinned(Scene* scene, int divisions, float size) {
    plane_init_triangle_list(scene, divisions, size);

    const int nVerticesSide = divisions + 1;
    const float tDivisionSize = 1.0f / (float)divisions;
    const Vec2 tBottomLeft = { 0.0f,1.0f };

    for( int y = 0,i = 0; y < nVerticesSide; y++ )
    {
        const float y_t = (float)-y * tDivisionSize;
        for( int x = 0; x < nVerticesSide; x++,i++ )
        {
            Vec2 temp = { (float)x * tDivisionSize,y_t };
            scene->triList->vertices[i].tc = vec2_add(&tBottomLeft, &temp);
        }
    }

}

void scene_plane_draw(Scene* scene, SDL_Renderer** renderer) {
    scene->pipeline->vertex_shader->time += 0.01f;
    scene->angle_x = PI/4;
    // clear z buffer
    pipeline_begin_frame(scene->pipeline);

   Mat proj = mat_projection_hfov(120.0f, 1.0f, 1.0f, 10.0f);

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

    // render triangles
    pipeline_draw(scene->pipeline, scene->triList);
}

Scene make_scene_plane(SDL_Renderer** renderer, const char* filename) {
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
    pipeline->zb = z_buffer_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    Scene scene;
    scene.angle_x = 0;
    scene.angle_y = 0;
    scene.angle_z = 0;
    scene.z_offset = 3;
    scene.time = 0.0f,
    
    pipeline->vertex_shader = create_wave_vertex_shader(scene.time);
    scene.pipeline = pipeline;

    return scene;
}