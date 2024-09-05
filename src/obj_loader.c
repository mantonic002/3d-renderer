#include "obj_loader.h"

// helper function to trim leading whitespace
char* trim_leading_whitespace(char* str) {
    while (isspace((unsigned char)*str)) str++;
    return str;
}

// helper function to trim trailing whitespace
char* trim_trailing_whitespace(char* str) {
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// function to load an .obj file
void load_obj(Scene* scene, const char* filename) {
    // make triList
    IndexedTriangleList* triangleList = malloc(sizeof(IndexedTriangleList));
    if (!triangleList) {
        fprintf(stderr, "Memory allocation failed for triangleList\n");
        exit(1);
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    int vertexCount = 0;
    int faceCount = 0;
    int normalCount = 0;
    int texCount = 0;
    char line[256];

    // count vertices and faces
    while (fgets(line, sizeof(line), file)) {
        char* trimmedLine = trim_leading_whitespace(trim_trailing_whitespace(line));
        if (trimmedLine[0] == 'v' && trimmedLine[1] == ' ') {
            vertexCount++;
        } else if (trimmedLine[0] == 'f') {
            faceCount++;
        } else if (trimmedLine[0] == 'v' && trimmedLine[1] == 'n') {
            normalCount++;
        } else if (trimmedLine[0] == 'v' && trimmedLine[1] == 't') {
            texCount++;
        }
    }

    // allocate memory for vertices and face indices
    Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    Vec3* indices = (Vec3*)malloc(sizeof(Vec3) * faceCount);
    Vec3* normals = (Vec3*)malloc(sizeof(Vec3) * normalCount);
    Vec2* texCoords = (Vec2*)malloc(sizeof(Vec2) * texCount);

    if (!vertices || !indices || !normals || !texCoords) {
        perror("Error allocating memory");
        fclose(file);
        exit(1);
    }

    // reset file pointer to the beginning
    fseek(file, 0, SEEK_SET);

    int vIndex = 0;
    int fIndex = 0;
    int nIndex = 0;
    int tIndex = 0;

    // read and store vertices and faces
    while (fgets(line, sizeof(line), file)) {
        char* trimmedLine = trim_leading_whitespace(trim_trailing_whitespace(line));
        if (trimmedLine[0] == 'v' && trimmedLine[1] == 'n') {
            // read vertex normals
            float x, y, z;
            sscanf(trimmedLine, "vn %f %f %f", &x, &y, &z);
            normals[nIndex] = (Vec3){x, y, z};
            nIndex++;
        } else if (trimmedLine[0] == 'v' && trimmedLine[1] == 't') {
            // read vertex normals
            float x, y;
            sscanf(trimmedLine, "vt %f %f", &x, &y);
            texCoords[tIndex] = (Vec2){x, y};
            tIndex++;
        } else if (trimmedLine[0] == 'v' && trimmedLine[1] == ' ') {
            // read vertex position
            float x, y, z;
            sscanf(trimmedLine, "v %f %f %f", &x, &y, &z);
            vertices[vIndex].pos = (Vec3){x, y, z};
            vIndex++;
        } else if (trimmedLine[0] == 'f') {
            // read face indices
            int v1, v2, v3, t1, t2, t3, n1, n2, n3;
            if (sscanf(trimmedLine, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &v1, &t1, &n1,
                       &v2, &t2, &n2,
                       &v3, &t3, &n3) == 9) {
                indices[fIndex++] = (Vec3){(float)v1-1, (float)v2-1, (float)v3-1};
                vertices[v1-1].n = normals[n1-1];
                vertices[v2-1].n = normals[n2-1];
                vertices[v3-1].n = normals[n3-1];

                vertices[v1-1].tc = texCoords[t1-1];
                vertices[v2-1].tc = texCoords[t2-1];
                vertices[v3-1].tc = texCoords[t3-1];
            } else {
                fprintf(stderr, "Warning: Non-triangular faces are not supported.\n");
            }
        }
    }

    fclose(file);

    // populate the triangleList structure
    triangleList->vertices = vertices;
    triangleList->sizeV = vertexCount;
    triangleList->indices = indices;
    triangleList->sizeI = faceCount;

    scene->triList = triangleList;
}
