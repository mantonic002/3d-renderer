#ifndef Z_BUFFER_H
#define Z_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

typedef struct {
    int width;
    int height;
    float* pBuffer;
} ZBuffer;

ZBuffer* z_buffer_init(int width, int height);
void z_buffer_free(ZBuffer* zbuf);
void z_buffer_clear(ZBuffer* zbuf);
float* z_buffer_at(ZBuffer* zbuf, int x, int y);
int z_buffer_test_and_set(ZBuffer* zbuf, int x, int y, float depth);

#endif