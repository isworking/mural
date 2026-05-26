/* canvas.h */

#pragma once

#ifndef MRL_CANVAS_H
#define MRL_CANVAS_H

#include <mrl/types.h>
#include <mrl/color.h>
#include <mrl/image.h>

MRL_BEGIN_DECLS

typedef struct mrl_canvas mrl_canvas;
typedef struct mrl_canvas_node mrl_canvas_node;
typedef struct mrl_drawable mrl_drawable;

MRL_API void mrl_canvas_node_set_drawable_clone(mrl_canvas_node *node, mrl_drawable *drawable);
MRL_API mrl_drawable *mrl_canvas_node_get_drawable(const mrl_canvas_node *node);

MRL_API void mrl_canvas_node_set_visible(mrl_canvas_node *node, bool visible);
MRL_API bool mrl_canvas_node_get_visible(const mrl_canvas_node *node);

MRL_API void mrl_canvas_node_set_opacity(mrl_canvas_node *node, float opacity);
MRL_API float mrl_canvas_node_get_opacity(const mrl_canvas_node *node);

MRL_API void mrl_canvas_node_set_bounds(mrl_canvas_node *node, mrl_rect bounds);
MRL_API mrl_rect mrl_canvas_node_get_bounds(const mrl_canvas_node *node);

MRL_API void mrl_canvas_node_set_position(mrl_canvas_node *node, mrl_pos position);
MRL_API mrl_pos mrl_canvas_node_get_position(const mrl_canvas_node *node);

MRL_API mrl_canvas *mrl_canvas_create(mrl_size size);
MRL_API mrl_canvas *mrl_canvas_create_from_image(mrl_image *image);

MRL_API mrl_size mrl_canvas_get_size(const mrl_canvas *canvas);
MRL_API int mrl_canvas_get_width(const mrl_canvas *canvas);
MRL_API int mrl_canvas_get_height(const mrl_canvas *canvas);

MRL_API mrl_color *mrl_canvas_get_pixels(const mrl_canvas *canvas);
MRL_API void mrl_canvas_set_pixel_blend(mrl_canvas *canvas, mrl_pos position, mrl_color color);
MRL_API mrl_color mrl_canvas_get_pixel(const mrl_canvas *canvas, mrl_pos position);

MRL_API void mrl_canvas_destroy(mrl_canvas *canvas);

MRL_API mrl_canvas_node *mrl_canvas_add(
    mrl_canvas *canvas,
    mrl_drawable *drawable,
    mrl_pos position);

MRL_API void mrl_canvas_render(
    mrl_canvas *canvas);

MRL_API void mrl_canvas_clear(
    mrl_canvas *canvas,
    mrl_color color);

MRL_API mrl_image *mrl_canvas_export_to_image(mrl_canvas *canvas);

MRL_END_DECLS

#endif