#include "draw_triangles_tex.h"

void draw_triangle_tex (Vertex* v1, Vertex* v2, Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // sort vertices by y (height)
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(Vertex));
    if (v3->pos.y < v2->pos.y) 
        ptr_swap(v2, v3, sizeof(Vertex));
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(Vertex));

    if (v1->pos.y == v2->pos.y) { // natural flat top
        if (v2->pos.x < v1->pos.x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(Vertex));
        draw_flat_top_triangle_tex(v1, v2, v3, s, renderer);
    }
    else if (v2->pos.y == v3->pos.y) { // natural flat bottom
        if (v3->pos.x < v2->pos.x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(Vertex));
        draw_flat_bottom_triangle_tex(v1, v2, v3, s, renderer);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
        // Interpolate to find the point between v3 and v1
        Vertex vi = vertex_interpolate(v1, v3, alpha);

        if (v2->pos.x < vi.pos.x) { // major right
            draw_flat_bottom_triangle_tex(v1, v2, &vi, s, renderer);
            draw_flat_top_triangle_tex(v2, &vi, v3, s, renderer);
        }
        else { // major left
            draw_flat_bottom_triangle_tex(v1, &vi, v2, s, renderer);
            draw_flat_top_triangle_tex(&vi, v2, v3, s, renderer);
        }
    }
}



void draw_flat_top_triangle_tex (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // calc dVertex / delta_y
    float delta_y = v3->pos.y - v1->pos.y;
    Vertex temp = vertex_subtract(v3, v1);
    Vertex dv1 = vertex_divide(&temp, delta_y);

    temp = vertex_subtract(v3, v2);
    Vertex dv2 = vertex_divide(&temp, delta_y);

    // right edge interpolants
    Vertex itEdge2 = *v2;

    draw_flat_triangle_tex(v1, v2, v3, s, renderer, &dv1, &dv2, &itEdge2);

}

void draw_flat_bottom_triangle_tex (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // calc dVertex / delta_y
    float delta_y = v3->pos.y - v1->pos.y;
    Vertex temp = vertex_subtract(v2, v1);
    Vertex dv1 = vertex_divide(&temp, delta_y);

    temp = vertex_subtract(v3, v1);
    Vertex dv2 = vertex_divide(&temp, delta_y);

    // right edge interpolant
    Vertex itEdge1 = *v1;
 
    draw_flat_triangle_tex(v1, v2, v3, s, renderer, &dv1, &dv2, &itEdge1);
}

void draw_flat_triangle_tex (const Vertex* v1, const Vertex* v2, const Vertex* v3, SDL_Surface* s, SDL_Renderer** renderer, const Vertex* dv1, const Vertex* dv2, Vertex* itEdge2) {
    // left edge interpolant
    Vertex itEdge1 = *v1;

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.y - 0.5f); // not drawn

    //interpolant prestep
    Vertex temp = vertex_multiply(dv1, ((float)yStart + 0.5f - v1->pos.y));
    itEdge1 = vertex_add(&itEdge1, &temp);

    temp = vertex_multiply(dv2, ((float)yStart + 0.5f - v1->pos.y));
    *itEdge2 = vertex_add(itEdge2, &temp);

    // init texture w/h and clamp values
    const float tex_width = (float)s->w;
    const float tex_height = (float)s->h;
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    Uint8 r, g, b, a;

    for (int y = yStart; y < yEnd; y++,
            itEdge1 = vertex_add(&itEdge1, dv1),
            *itEdge2 = vertex_add(itEdge2, dv2)) 
    {
        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(itEdge1.pos.x - 0.5f);
        int xEnd = (int)ceil(itEdge2->pos.x - 0.5f); // not drawn

        // scanline dTexCoord / dx
        Vec2 temp = vec2_subtract(&itEdge2->tc, &itEdge1.tc);
        Vec2 dtcLine = vec2_divide(&temp, (itEdge2->pos.x - itEdge1.pos.x));

        // scanline texture coordinate interpolant and prestep
        temp = vec2_multiply(&dtcLine, ((float)xStart + 0.5f - itEdge1.pos.x));
        Vec2 itcLine = vec2_add(&itEdge1.tc, &temp);

        for (int x = xStart; x < xEnd; x++, itcLine = vec2_add(&itcLine, &dtcLine)) {
            get_pixel_rgba( s, 
                            fmin(itcLine.x * tex_width, tex_clamp_x),
                            fmin(itcLine.y * tex_height, tex_clamp_y),
                            &r, &g, &b, &a);
            SDL_SetRenderDrawColor(*renderer, r, g, b, a);
            SDL_RenderDrawPoint(*renderer, x, y);
        }
    }
}

