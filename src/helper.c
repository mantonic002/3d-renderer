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

SDL_Color get_pixel_rgba(SDL_Surface *surface, int x, int y) {
    Uint32 pixel_value = get_pixel(surface, x, y);
    SDL_Color c;
    SDL_GetRGBA(pixel_value, surface->format, &c.r, &c.g, &c.b, &c.a);
    return c;
}

void put_pixel(SDL_Surface *surface, int x, int y, SDL_Color* c) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        return; // out of bounds
    }

    // check if the surface format is 32-bit
    if (surface->format->BytesPerPixel != 4) {
        return;
    }

    // get the address of the pixel
    Uint32* pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * sizeof(Uint32));

    Uint32 color32 = SDL_MapRGB(surface->format, c->r, c->g, c->b);
    *pixel = color32;
}

void clear_surface(SDL_Surface* surface) {
    if (surface == NULL) {
        return;
    }

    // create a SDL_Rect that covers the entire surface
    SDL_Rect rect = {0, 0, surface->w, surface->h};

    Uint32 c = SDL_MapRGB(surface->format, 0, 0, 0);

    // fill the surface with the black color
    SDL_FillRect(surface, &rect, c);
}