#include "pipeline.h"

void pipeline_draw(Pipeline* p, IndexedTriangleList* triList) {
    process_vertices (p, triList->vertices, triList->sizeV, triList->indices, triList->sizeI);
}

void pipeline_begin_frame(Pipeline* p) {
    z_buffer_clear(p->zb);
}

void process_vertices (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI) {
    Vertex verticesOut[sizeV];

    for (int i = 0; i < sizeV; i++) {
        verticesOut[i] = p->vertex_shader->apply(p->vertex_shader, &vertices[i]);
    }

    assemble_triangles(p, verticesOut, sizeV, indices, sizeI);
}

void assemble_triangles (Pipeline* p, const Vertex* vertices, int sizeV, const Vec3* indices, int sizeI) {
    // assemble triangles in the stream and process
    Vec4 temp = (Vec4){0.0f, 0.0f, 0.0f, 1.0f};
    temp = multiply_matrix_by_vec4(p->vertex_shader->proj, &temp);
    Vec3 eyepos = vec3(&temp);

    for (int i = 0; i < sizeI; i++) {
        Vertex v1 = vertices[(int)indices[i].x];
        Vertex v2 = vertices[(int)indices[i].y];
        Vertex v3 = vertices[(int)indices[i].z];

        // backface culling
        Vec3 v2_sub_v1 = vec3_subtract(&v2.pos.as_vec3, &v1.pos.as_vec3);
        Vec3 v3_sub_v1 = vec3_subtract(&v3.pos.as_vec3, &v1.pos.as_vec3);
        Vec3 n = cross_product(&v2_sub_v1, &v3_sub_v1);
        Vec3 temp = vec3_subtract(&v1.pos.as_vec3, &eyepos);
        if (dot_product(&n, &temp) <= 0.0f) {
            process_triangle(p, &v1, &v2, &v3, i);
        }
    }
}

void process_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3, int triangle_index) {
    Triangle t = p->geometry_shader->shade(p->geometry_shader, v1, v2, v3, triangle_index);
    post_process_triangle(p, &t);
}

void post_process_triangle (Pipeline* p, Triangle* triangle) {
    transform(&triangle->v1);
    transform(&triangle->v2);
    transform(&triangle->v3);

    draw_triangle(p, triangle);
}

void draw_triangle (Pipeline* p, Triangle* triangle) {
    Vertex* v1 = &triangle->v1;
    Vertex* v2 = &triangle->v2;
    Vertex* v3 = &triangle->v3;

    // sort vertices by y (height)
    if (v2->pos.as_vec4.y < v1->pos.as_vec4.y) 
        ptr_swap(v1, v2, sizeof(Vertex));
    if (v3->pos.as_vec4.y < v2->pos.as_vec4.y) 
        ptr_swap(v2, v3, sizeof(Vertex));
    if (v2->pos.as_vec4.y < v1->pos.as_vec4.y) 
        ptr_swap(v1, v2, sizeof(Vertex));

    if (v1->pos.as_vec4.y == v2->pos.as_vec4.y) { // natural flat top
        if (v2->pos.as_vec4.x < v1->pos.as_vec4.x) // sort top vertices by x
            ptr_swap(v1, v2, sizeof(Vertex));

        draw_flat_top_triangle(p, v1, v2, v3);
    }
    else if (v2->pos.as_vec4.y == v3->pos.as_vec4.y) { // natural flat bottom
        if (v3->pos.as_vec4.x < v2->pos.as_vec4.x) // sort bottom vertices by x
            ptr_swap(v2, v3, sizeof(Vertex));

        draw_flat_bottom_triangle(p, v1, v2, v3);
    }
    else { // general case - has to be split into 1 flat top and 1 flat bottom
        // find a splitting vertex y
        float alpha = (v2->pos.as_vec4.y - v1->pos.as_vec4.y) / (v3->pos.as_vec4.y - v1->pos.as_vec4.y);
        // Interpolate to find the point between v3 and v1
        Vertex vi = vertex_interpolate(v1, v3, alpha);

        if (v2->pos.as_vec4.x < vi.pos.as_vec4.x) { // major right
            draw_flat_bottom_triangle(p, v1, v2, &vi);
            draw_flat_top_triangle(p, v2, &vi, v3);
        }
        else { // major left
            draw_flat_bottom_triangle(p, v1, &vi, v2);
            draw_flat_top_triangle(p, &vi, v2, v3);
        }
    }
}

void draw_flat_top_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3) {
    // calc dVertex / delta_y
    float delta_y = v3->pos.as_vec4.y - v1->pos.as_vec4.y;
    Vertex temp = vertex_subtract(v3, v1);
    Vertex dv1 = vertex_divide(&temp, delta_y);

    temp = vertex_subtract(v3, v2);
    Vertex dv2 = vertex_divide(&temp, delta_y);

    // right edge interpolants
    Vertex itEdge2 = *v2;

    draw_flat_triangle(p, v1, v2, v3, &dv1, &dv2, &itEdge2);

}

void draw_flat_bottom_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3) {
    // calc dVertex / delta_y
    float delta_y = v3->pos.as_vec4.y - v1->pos.as_vec4.y;
    Vertex temp = vertex_subtract(v2, v1);
    Vertex dv1 = vertex_divide(&temp, delta_y);

    temp = vertex_subtract(v3, v1);
    Vertex dv2 = vertex_divide(&temp, delta_y);

    // right edge interpolant
    Vertex itEdge1 = *v1;
 
    draw_flat_triangle(p, v1, v2, v3, &dv1, &dv2, &itEdge1);
}

void draw_flat_triangle (Pipeline* p, const Vertex* v1, const Vertex* v2, const Vertex* v3, const Vertex* dv1, const Vertex* dv2, Vertex* itEdge2) {
    // left edge interpolant
    Vertex itEdge1 = *v1;

    // TOP part of the top-left rule
    // start and end scanlines
    int yStart = (int)ceil( v1->pos.as_vec4.y - 0.5f);
    int yEnd = (int)ceil( v3->pos.as_vec4.y - 0.5f); // not drawn

    //interpolant prestep
    Vertex temp = vertex_multiply(dv1, ((float)yStart + 0.5f - v1->pos.as_vec4.y));
    itEdge1 = vertex_add(&itEdge1, &temp);

    temp = vertex_multiply(dv2, ((float)yStart + 0.5f - v1->pos.as_vec4.y));
    *itEdge2 = vertex_add(itEdge2, &temp);

    SDL_Color c;
    for (int y = yStart; y < yEnd; y++,
            itEdge1 = vertex_add(&itEdge1, dv1),
            *itEdge2 = vertex_add(itEdge2, dv2)) 
    {
        // LEFT part of the top-left rule
        // calc start and end pixels
        int xStart = (int)ceil(itEdge1.pos.as_vec4.x - 0.5f);
        int xEnd = (int)ceil(itEdge2->pos.as_vec4.x - 0.5f); // not drawn

        // scanline interpolant startpoint
        Vertex iLine = itEdge1;

        // delta scanline interpolant/dx
        const float dx = itEdge2->pos.as_vec4.x - itEdge1.pos.as_vec4.x;
        temp = vertex_subtract(itEdge2, &iLine);
        Vertex diLine = vertex_divide(&temp, dx);

        temp = vertex_multiply(&diLine, ((float)xStart + 0.5f - itEdge1.pos.as_vec4.x));
        iLine = vertex_add(&iLine, &temp);

        for (int x = xStart; x < xEnd; x++, iLine = vertex_add(&iLine, &diLine)) {

            if (z_buffer_test_and_set(p->zb, x, y, iLine.pos.as_vec4.z)){   
                // recover interpolated z from interpolated zInv
                float w = 1.0f / iLine.pos.as_vec4.w;

                // recover interpolated atributes
                Vertex attr = vertex_multiply(&iLine, w);

                // texture lookup and clamp with pixel shader and returned draw pixel
                c = p->pixel_shader->shade(p->pixel_shader, &attr);

                SDL_SetRenderDrawColor(*p->renderer, c.r, c.g, c.b, c.a);
                SDL_RenderDrawPoint(*p->renderer, x, y);
            }
        }
    }
}

// transformation from object space to screen space
void transform (Vertex* v) {   
        // perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
        float wInv = 1.0f / v->pos.as_vec4.w;

        // dividing all components of vertex by w, including texture coordinates
        *v = vertex_multiply(v, wInv);
        
        // adjust x and y with window width and scale
        v->pos.as_vec4.x = WINDOW_WIDTH/2 + v->pos.as_vec4.x * SCALE;
        v->pos.as_vec4.y = WINDOW_HEIGHT/2 + v->pos.as_vec4.y * SCALE;

        // store wInv because everything was divided by w including w itself.
        v->pos.as_vec4.w = wInv;
}
