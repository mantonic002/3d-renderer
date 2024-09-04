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