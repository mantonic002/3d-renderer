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
        return 0; 
    }

    // pixel's position in the surface's pixel buffer
    int bpp = surface->format->BytesPerPixel;
    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

    Uint32 pixel_value;
    memcpy(&pixel_value, pixel_ptr, bpp);

    return pixel_value;
}

void get_pixel_rgba(SDL_Surface* surface, int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a) {
    Uint32 pixel_value = get_pixel(surface, x, y);
    SDL_GetRGBA(pixel_value, surface->format, r, g, b, a);
}