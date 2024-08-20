#include "draw_triangles_tex.h"

void draw_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // sort vertices by y (height)
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(TexVertex));
    if (v3->pos.y < v2->pos.y) 
        ptr_swap(v2, v3, sizeof(TexVertex));
    if (v2->pos.y < v1->pos.y) 
        ptr_swap(v1, v2, sizeof(TexVertex));

    if (v1->pos.y == v2->pos.y) { // natural flat top
        if (v2->pos.x < v1->pos.x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(TexVertex));
        draw_flat_top_triangle_tex(v1, v2, v3, s, renderer);
    }
    else if (v2->pos.y == v3->pos.y) { // natural flat bottom
        if (v3->pos.x < v2->pos.x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(TexVertex));
        draw_flat_bottom_triangle_tex(v1, v2, v3, s, renderer);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
        // Interpolate to find the point between v3 and v1
        TexVertex vi = tex_vertex_interpolate(v1, v3, alpha);

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



void draw_flat_top_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v3->pos.x - v1->pos.x) / (v3->pos.y - v1->pos.y);
    float m2 = (v3->pos.x - v2->pos.x) / (v3->pos.y - v2->pos.y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.y - 0.5f); // not drawn

    // texture coordinate points
    Vec2 tcEdgeL = v1->tc;
    Vec2 tcEdgeR = v2->tc;
    Vec2 tcBottom = v3->tc;

    // texture coordinate edge unit step
    Vec2 b_sub_l = vec2_subtract(&tcBottom, &tcEdgeL);
    const Vec2 tcEdgeStepL = vec2_divide(&b_sub_l, (v3->pos.y - v1->pos.y));

    Vec2 b_sub_r = vec2_subtract(&tcBottom, &tcEdgeR);
    const Vec2 tcEdgeStepR = vec2_divide(&b_sub_r, (v3->pos.y - v2->pos.y));

    // add texture edge prestep
    Vec2 prestep = vec2_multiply(&tcEdgeStepL, ((float)yStart + 0.5f - v2->pos.y));
    tcEdgeL = vec2_add(&tcEdgeL, &prestep);

    prestep = vec2_multiply(&tcEdgeStepR, ((float)yStart + 0.5f - v2->pos.y));
    tcEdgeR = vec2_add(&tcEdgeR, &prestep);

    // init texture w/h and clamp values
    const float tex_width = (float)s->w;
    const float tex_height = (float)s->h;
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    Uint8 r, g, b, a;

    for (int y = yStart; y < yEnd; y++,
            tcEdgeL = vec2_add(&tcEdgeL, &tcEdgeStepL),
            tcEdgeR = vec2_add(&tcEdgeR, &tcEdgeStepR)) 
    {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;
        float x2 = m2 * ((float)y + 0.5f - v2->pos.y) + v2->pos.x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        // texture coordinate scanline unit step
        Vec2 temp = vec2_subtract(&tcEdgeR, &tcEdgeL);
        const Vec2 tcScanStep = vec2_divide(&temp, (x2 - x1));

        temp = vec2_multiply(&tcScanStep, ((float)xStart + 0.5f - x1));
        Vec2 tc = vec2_add(&tcEdgeL, &temp);

        for (int x = xStart; x < xEnd; x++, tc = vec2_add(&tc, &tcScanStep)) {
            get_pixel_rgba( s, 
                            fmin(tc.x * tex_width, tex_clamp_x),
                            fmin(tc.y * tex_height, tex_clamp_y),
                            &r, &g, &b, &a);
            SDL_SetRenderDrawColor(*renderer, r, g, b, a);
            SDL_RenderDrawPoint(*renderer, x, y);
        }
    }
}


void draw_flat_bottom_triangle_tex (TexVertex* v1, TexVertex* v2, TexVertex* v3, SDL_Surface* s, SDL_Renderer** renderer) {
    // calculate slopes (inverse because Xs could be the same if it was horizontal, which would cause devision by 0)
    float m1 = (v2->pos.x - v1->pos.x) / (v2->pos.y - v1->pos.y);
    float m2 = (v3->pos.x - v1->pos.x) / (v3->pos.y - v1->pos.y);

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.y - 0.5f); // not drawn

    // texture coordinate points
    Vec2 tcEdgeL = v1->tc;
    Vec2 tcEdgeR = v1->tc;
    Vec2 tcBottomL = v2->tc;
    Vec2 tcBottomR = v3->tc;

    // texture coordinate edge unit step
    Vec2 b_sub_l = vec2_subtract(&tcBottomL, &tcEdgeL);
    const Vec2 tcEdgeStepL = vec2_divide(&b_sub_l, (v2->pos.y - v1->pos.y));

    Vec2 b_sub_r = vec2_subtract(&tcBottomR, &tcEdgeR);
    const Vec2 tcEdgeStepR = vec2_divide(&b_sub_r, (v3->pos.y - v1->pos.y));

    // add texture edge prestep
    Vec2 prestep = vec2_multiply(&tcEdgeStepL, ((float)yStart + 0.5f - v1->pos.y));
    tcEdgeL = vec2_add(&tcEdgeL, &prestep);

    prestep = vec2_multiply(&tcEdgeStepR, ((float)yStart + 0.5f - v1->pos.y));
    tcEdgeR = vec2_add(&tcEdgeR, &prestep);

    // init texture w/h and clamp values
    const float tex_width = (float)s->w;
    const float tex_height = (float)s->h;
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    for (int y = yStart; y < yEnd; y++,
            tcEdgeL = vec2_add(&tcEdgeL, &tcEdgeStepL),
            tcEdgeR = vec2_add(&tcEdgeR, &tcEdgeStepR)) 
    {
        // calc Xs for start and end vertices
        // add 0.5 because it's calculater from pixel centers
        float x1 = m1 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;
        float x2 = m2 * ((float)y + 0.5f - v1->pos.y) + v1->pos.x;

        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(x1 - 0.5f);
        int xEnd = (int)ceil(x2 - 0.5f); // not drawn

        // texture coordinate scanline unit step
        Vec2 temp = vec2_subtract(&tcEdgeR, &tcEdgeL);
        const Vec2 tcScanStep = vec2_divide(&temp, (x2 - x1));

        temp = vec2_multiply(&tcScanStep, ((float)xStart + 0.5f - x1));
        Vec2 tc = vec2_add(&tcEdgeL, &temp);

        for (int x = xStart; x < xEnd; x++, tc = vec2_add(&tc, &tcScanStep)) {
            Uint8 r, g, b, a;
            get_pixel_rgba( s, 
                            fmin(tc.x * tex_width, tex_clamp_x),
                            fmin(tc.y * tex_height, tex_clamp_y),
                            &r, &g, &b, &a);

            SDL_SetRenderDrawColor(*renderer, r, g, b, a);
            SDL_RenderDrawPoint(*renderer, x, y);
        }
    }
}
