#include "z_buffer.h"

ZBuffer* z_buffer_init(int width, int height) {
    ZBuffer* zbuf = (ZBuffer*)malloc(sizeof(ZBuffer));
    if (zbuf == NULL) {
        perror("Failed to allocate ZBuffer");
        exit(1);
    }
    zbuf->width = width;
    zbuf->height = height;
    zbuf->pBuffer = (float*)malloc(width * height * sizeof(float));
    if (zbuf->pBuffer == NULL) {
        perror("Failed to allocate ZBuffer buffer");
        free(zbuf);
        exit(1);
    }
    z_buffer_clear(zbuf);
    return zbuf;
}

void z_buffer_free(ZBuffer* zbuf) {
    if (zbuf != NULL) {
        free(zbuf->pBuffer);
        free(zbuf);
    }
}

void z_buffer_clear(ZBuffer* zbuf) {
    int nDepths = zbuf->width * zbuf->height;
    for (int i = 0; i < nDepths; i++) {
        zbuf->pBuffer[i] = FLT_MAX;
    }
}

float* z_buffer_at(ZBuffer* zbuf, int x, int y) {
    if (x < 0 || x >= zbuf->width || y < 0 || y >= zbuf->height) {
        fprintf(stderr, "Index out of bounds: (%d, %d)\n", x, y);
        exit(1);
    }
    return &zbuf->pBuffer[y * zbuf->width + x];
}

int z_buffer_test_and_set(ZBuffer* zbuf, int x, int y, float depth) {
    float* depthInBuffer = z_buffer_at(zbuf, x, y);
    if (depth < *depthInBuffer) {
        *depthInBuffer = depth;
        return 1; // depth updated
    }
    return 0; // depth not updated
}