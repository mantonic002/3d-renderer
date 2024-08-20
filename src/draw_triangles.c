#include "draw_triangles.h"

void draw_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer) {
    // sort vertices by y (height)
    if (v2->y < v1->y) 
        ptr_swap(v1, v2, sizeof(Vec2));
    if (v3->y < v2->y) 
        ptr_swap(v2, v3, sizeof(Vec2));
    if (v2->y < v1->y) 
        ptr_swap(v1, v2, sizeof(Vec2));

    if (v1->y == v2->y) { // natural flat top
        if (v2->x < v1->x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(Vec2));
        draw_flat_top_triangle(v1, v2, v3, c, renderer);
    }
    else if (v2->y == v3->y) { // natural flat bottom
        if (v3->x < v2->x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(Vec2));
        draw_flat_bottom_triangle(v1, v2, v3, c, renderer);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->y - v1->y) / (v3->y - v1->y);
        // Interpolate to find the point between v3 and v1
        Vec2 vi = vec2_interpolate(v1, v3, alpha);

        if (v2->x < vi.x) { // major right
            draw_flat_bottom_triangle(v1, v2, &vi, c, renderer);
            draw_flat_top_triangle(v2, &vi, v3, c, renderer);
        }
        else { // major left
            draw_flat_bottom_triangle(v1, &vi, v2, c, renderer);
            draw_flat_top_triangle(&vi, v2, v3, c, renderer);
        }
    }
}

void draw_flat_top_triangle (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v3->x - v1->x) / (v3->y - v1->y);
    float m2 = (v3->x - v2->x) / (v3->y - v2->y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->y - 0.5f);
    int yEnd = (int)ceil( v3->y - 0.5f); // not drawn

    for (int y = yStart; y < yEnd; y++) {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->y) + v1->x;
        float x2 = m2 * ((float)y + 0.5f - v2->y) + v2->x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        for (int x = xStart; x < xEnd; x++) {
            SDL_SetRenderDrawColor(*renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(*renderer, x, y);
        }
    }
}

void draw_flat_bottom_triangle  (Vec2* v1, Vec2* v2, Vec2* v3, SDL_Color c, SDL_Renderer** renderer) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v2->x - v1->x) / (v2->y - v1->y);
    float m2 = (v3->x - v1->x) / (v3->y - v1->y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->y - 0.5f);
    int yEnd = (int)ceil( v3->y - 0.5f); // not drawn

    for (int y = yStart; y < yEnd; y++) {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->y) + v1->x;
        float x2 = m2 * ((float)y + 0.5f - v1->y) + v1->x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        for (int x = xStart; x < xEnd; x++) {
            SDL_SetRenderDrawColor(*renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(*renderer, x, y);
        }
    }
}
