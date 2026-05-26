#pragma once

#ifndef MRL_CANVAS_INTERNAL_H
#define MRL_CANVAS_INTERNAL_H

#include <mrl/canvas.h>
#include <mrl/drawable.h>

#define MRL_CANVAS_NODE_PAGE_CAPACITY 64

typedef struct mrl_canvas_node
{
    mrl_drawable *drawable;

    bool visible;
    float opacity;
    mrl_rect bounds;
} mrl_canvas_node;

typedef struct mrl_canvas_node_page
{
    mrl_canvas_node nodes[MRL_CANVAS_NODE_PAGE_CAPACITY];

    size_t used;

    struct mrl_canvas_node_page *next;
} mrl_canvas_node_page;

struct mrl_canvas
{
    mrl_size size;

    mrl_color *pixels;

    mrl_canvas_node_page *pages;
    mrl_canvas_node_page *last_page;

    size_t node_count;
};

#endif