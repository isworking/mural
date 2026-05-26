#include <mrl/canvas.h>
#include <mrl/image.h>

#include "internal/canvas_internal.h"

#include <stdlib.h>
#include <string.h>

static mrl_canvas_node_page *mrl_canvas_alloc_page(void)
{
    mrl_canvas_node_page *page =
        malloc(sizeof(*page));

    if (!page)
        return NULL;

    page->used = 0;
    page->next = NULL;

    return page;
}

void mrl_canvas_node_set_drawable_clone(mrl_canvas_node *node, mrl_drawable *drawable)
{
    node->drawable = mrl_drawable_clone(drawable);
}

mrl_drawable *mrl_canvas_node_get_drawable(const mrl_canvas_node *node)
{
    return node->drawable;
}

void mrl_canvas_node_set_visible(mrl_canvas_node *node, bool visible)
{
    node->visible = visible;
}

bool mrl_canvas_node_get_visible(const mrl_canvas_node *node)
{
    return node->visible;
}

void mrl_canvas_node_set_opacity(mrl_canvas_node *node, float opacity)
{
    node->opacity = opacity;
}

float mrl_canvas_node_get_opacity(const mrl_canvas_node *node)
{
    return node->opacity;
}

void mrl_canvas_node_set_bounds(mrl_canvas_node *node, mrl_rect bounds)
{
    node->bounds = bounds;
}

mrl_rect mrl_canvas_node_get_bounds(const mrl_canvas_node *node)
{
    return node->bounds;
}

void mrl_canvas_node_set_position(mrl_canvas_node *node, mrl_pos position)
{
    mrl_drawable *drawable = mrl_canvas_node_get_drawable(node);

    node->bounds = mrl_drawable_bounds(drawable, position);
}

mrl_pos mrl_canvas_node_get_position(const mrl_canvas_node *node)
{
    mrl_rect bounds = mrl_canvas_node_get_bounds(node);

    return MRL_POS(bounds.x, bounds.y);
}

mrl_canvas *mrl_canvas_create(mrl_size size)
{
    mrl_canvas *canvas = malloc(sizeof(mrl_canvas));

    if (!canvas)
        return NULL;

    canvas->size = size;

    canvas->pixels = malloc(sizeof(mrl_color) * size.w * size.h);

    if (!canvas->pixels)
    {
        free(canvas);
        return NULL;
    }

    memset(canvas->pixels, 0,
           sizeof(mrl_color) * size.w * size.h);

    canvas->pages = NULL;
    canvas->last_page = NULL;

    return canvas;
}

mrl_canvas *mrl_canvas_create_from_image(
    mrl_image *img)
{
    if (!img)
        return NULL;

    mrl_canvas *canvas =
        mrl_canvas_create(
            mrl_image_get_size(img));

    if (!canvas)
        return NULL;

    size_t total =
        (size_t)mrl_image_get_width(img) *
        (size_t)mrl_image_get_height(img);

    memcpy(
        canvas->pixels,
        mrl_image_get_pixels(img),
        total * sizeof(*canvas->pixels));

    return canvas;
}

mrl_size mrl_canvas_get_size(const mrl_canvas *canvas)
{
    return canvas->size;
}

int mrl_canvas_get_width(const mrl_canvas *canvas)
{
    return mrl_canvas_get_size(canvas).w;
}

int mrl_canvas_get_height(const mrl_canvas *canvas)
{
    return mrl_canvas_get_size(canvas).h;
}

mrl_color *mrl_canvas_get_pixels(const mrl_canvas *canvas)
{
    return canvas->pixels;
}

void mrl_canvas_set_pixel_blend(mrl_canvas *canvas, mrl_pos position, mrl_color color)
{
    mrl_canvas_get_pixels(canvas)[position.y * canvas->size.w + position.x] = mrl_color_blend(color, mrl_canvas_get_pixels(canvas)[position.y * canvas->size.w + position.x]);
}

mrl_color mrl_canvas_get_pixel(const mrl_canvas *canvas, mrl_pos position)
{
    return mrl_canvas_get_pixels(canvas)[position.y * canvas->size.w + position.x];
}

void mrl_canvas_destroy(mrl_canvas *canvas)
{
    mrl_canvas_node_page *page = canvas->pages;

    while (page)
    {
        for (size_t i = 0; i < page->used; i++)
        {
            mrl_drawable_destroy(
                page->nodes[i].drawable);
        }

        mrl_canvas_node_page *next =
            page->next;

        free(page);

        page = next;
    }

    if (!canvas)
        return;

    free(canvas->pixels);
    free(canvas);
}

mrl_canvas_node *mrl_canvas_add(
    mrl_canvas *canvas,
    mrl_drawable *drawable, mrl_pos position)
{
    if (!canvas || !drawable)
        return NULL;

    mrl_canvas_node_page *page = canvas->last_page;

    if (!page || page->used >= MRL_CANVAS_NODE_PAGE_CAPACITY)
    {
        mrl_canvas_node_page *new_page = mrl_canvas_alloc_page();

        if (!new_page)
            return NULL;

        if (!canvas->pages)
            canvas->pages = new_page;
        else
            canvas->last_page->next = new_page;

        canvas->last_page = new_page;
        page = new_page;
    }

    mrl_canvas_node *node = &page->nodes[page->used++];

    memset(node, 0, sizeof(*node));

    node->drawable = mrl_drawable_clone(drawable);
    node->visible = true;
    node->opacity = 1.0f;
    node->bounds = mrl_drawable_bounds(drawable, position);

    return node;
}

void mrl_canvas_render(mrl_canvas *canvas)
{
    mrl_color *pixels = mrl_canvas_get_pixels(canvas);

    int canvas_width = mrl_canvas_get_width(canvas);
    int canvas_height = mrl_canvas_get_height(canvas);

    for (mrl_canvas_node_page *page = canvas->pages; page; page = page->next)
    {
        for (size_t i = 0; i < page->used; i++)
        {
            mrl_canvas_node *current_node = &page->nodes[i];

            mrl_rect bounds = mrl_canvas_node_get_bounds(current_node);

            if (bounds.w <= 0 || bounds.h <= 0)
                continue;

            int start_x =
                bounds.x < 0 ? 0 : bounds.x;

            int start_y =
                bounds.y < 0 ? 0 : bounds.y;

            int end_x =
                bounds.x + bounds.w;

            int end_y =
                bounds.y + bounds.h;

            if (end_x > canvas_width)
                end_x = canvas_width;

            if (end_y > canvas_height)
                end_y = canvas_height;

            bool visible = mrl_canvas_node_get_visible(current_node);
            float opacity = mrl_canvas_node_get_opacity(current_node);

            if (visible)
            {
                mrl_drawable *drawable = mrl_canvas_node_get_drawable(current_node);

                for (int y = start_y; y < end_y; y++)
                {
                    int row = y * canvas_width;

                    for (int x = start_x; x < end_x; x++)
                    {
                        int idx = row + x;

                        int local_x = x - bounds.x;
                        int local_y = y - bounds.y;
                        mrl_pos local_pos = MRL_POS(local_x, local_y);

                        mrl_color out;

                        if (mrl_drawable_sample(drawable, canvas, local_pos, &out))
                        {
                            out.a = (mrl_u8)((float)out.a * opacity);

                            pixels[idx] = mrl_color_blend(out, pixels[idx]);
                        }
                    }
                }
            }
        }
    }
}

void mrl_canvas_clear(
    mrl_canvas *canvas,
    mrl_color color)
{
    if (!canvas)
        return;

    mrl_color *pixels = mrl_canvas_get_pixels(canvas);

    if (!pixels)
        return;

    size_t total =
        (size_t)canvas->size.w *
        (size_t)canvas->size.h;

    for (size_t i = 0; i < total; i++)
        pixels[i] = color;
}

mrl_image *mrl_canvas_export_to_image(
    mrl_canvas *canvas)
{
    if (!canvas)
        return NULL;

    mrl_image *img = mrl_image_create(canvas->size);

    if (!img)
        return NULL;

    mrl_image_set_pixels_copy(img, canvas->pixels);

    return img;
}