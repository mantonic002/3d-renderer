#include "helper.h"

// helper func for swapping pointers
void ptr_swap(void* p1, void* p2, size_t size) {
    void* temp = malloc(size);
    if (temp == NULL) {
        return;
    }
    
    memcpy(temp, p1, size);
    memcpy(p1, p2, size);  
    memcpy(p2, temp, size);
    
    free(temp);
}

Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
    // check if coordinates are within the bounds of the surface
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        printf("Pixel out of bounds (%d, %d)", x, y);
        return 0; 
    }

    // pixel's position in the surface's pixel buffer
    int bpp = surface->format->BytesPerPixel;
    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

    Uint32 pixel_value;
    memcpy(&pixel_value, pixel_ptr, bpp);

    return pixel_value;
}

SDL_Color get_pixel_rgba(SDL_Surface *surface, int x, int y) {
    Uint32 pixel_value = get_pixel(surface, x, y);
    SDL_Color c;
    SDL_GetRGBA(pixel_value, surface->format, &c.r, &c.g, &c.b, &c.a);
    return c;
}

Vec3 multiply_matrix_by_point(const float matrix[3][3], const Vec3* point) {
    Vec3 result;
    result.x = (point->x * matrix[0][0]) + (point->y * matrix[1][0]) + (point->z * matrix[2][0]);
    result.y = (point->x * matrix[0][1]) + (point->y * matrix[1][1]) + (point->z * matrix[2][1]);
    result.z = (point->x * matrix[0][2]) + (point->y * matrix[1][2]) + (point->z * matrix[2][2]);
    return result;
}


