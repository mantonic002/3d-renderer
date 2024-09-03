#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "vec3.h"
#include "vec2.h"
#include "vertex.h"
#include "indexed_triangle_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* trim_leading_whitespace(char* str);
char* trim_trailing_whitespace(char* str);
void load_obj(const char* filename, IndexedTriangleList* triangleList);

#endif