# Mural

A lightweight 2D raster graphics and canvas library written in C.

> [!WARNING]
> Mural is under active development. The API is unstable and may change without notice.

## Features

- Image-backed canvases
- Drawable primitives
- Alpha compositing
- PNG and PPM import/export
- Retained-mode rendering

## Example

```c
#include <mrl/mrl.h>

int main(void)
{
    mrl_canvas *canvas = mrl_canvas_create(MRL_SIZE(800, 600));

    mrl_drawable *rect = mrl_drawable_rect(MRL_SIZE(200, 100));

    mrl_drawable_set_color(rect, mrl_color_from_hex_rgba(0xFF000080));
    mrl_canvas_add(canvas, rect, MRL_POS(100, 100));

    mrl_drawable_set_color(rect, mrl_color_from_hex_rgba(0x00FF0080));
    mrl_canvas_add(canvas, rect, MRL_POS(200, 150));

    mrl_drawable_set_color(rect, mrl_color_from_hex_rgba(0x00FFFF80));
    mrl_canvas_add(canvas, rect, MRL_POS(225, 175));

    mrl_canvas_render(canvas);

    mrl_image *out = mrl_canvas_export_to_image(canvas);
    mrl_image_write_png(out, "output.png");
    mrl_image_destroy(out);

    mrl_canvas_destroy(canvas);

    mrl_drawable_destroy(rect);

    return 0;
}
```

This should result in the following image:
![Mural screenshot](examples/output.png)

See [examples/](examples/) for more.

## Dependencies

- `libpng` (optional, disable with `MRL_ENABLE_PNG=OFF`)

## Build

```sh
make
```

Or manually with CMake:

```sh
cmake -B build -S .
cmake --build build
```

## Status

Early development. The API is unstable and tests are still being written.

## Roadmap

- [x] ~~Image drawables~~
    - [ ] (optional) Use `state.color` as tint

- [x] ~~Basic color parsing~~
    - [x] ~~RGBA~~
    - [x] ~~RGB~~
    - [x] ~~Hexadecimal~~

- [x] ~~Drawable bounding boxes~~

- [x] ~~Rasterize per drawable instead of per pixel~~

- [ ] Moving from LinkedList to Paged Allocators for Canvas Nodes

- [ ] Error handling
    - [ ] Error messages
    - [ ] Error types
    - [ ] Error handling helper macros and functions
    - [ ] (optional) `errno` integration

- [ ] Add tests
    - [ ] Basic tests
    - [ ] Mandatory tests
    - [ ] Stability tests
    - [ ] ABI tests
    - [ ] Additional tests

- [ ] Add color constants
    - [ ] Basic common color constants
    - [ ] (optional) Additional color constants

- [ ] Improve documentation and examples

- [ ] Blending mode support
    - [ ] Normal blending
    - [ ] Additive blending
    - [ ] Multiply blending
    - [ ] Configurable blending

- [ ] Clipping/scissor regions

- [ ] Image format support
    - [x] ~~PNG~~
        - [ ] Add more detailed PNG metadata
    - [ ] JPEG
    - [ ] (optional) BMP

- [ ] Introduce format-aware color representation
    - [ ] Generic pixel format abstraction
    - [ ] RGBA8888 support
    - [ ] RGB888 support

- [ ] Add pixel storage format support
    - [ ] BGRA8888
    - [ ] RGB565
    - [ ] GRAY8
    - [ ] Configurable pixel storage format

- [ ] Add color space support
    - [x] ~~sRGB~~
    - [ ] (optional) Linear RGB
    - [ ] Configurable color space

- [ ] Add color model support
    - [x] ~~RGB~~
    - [ ] HSL
    - [ ] HSV
    - [ ] (optional) Grayscale
    - [ ] (optional) CMYK
    - [ ] Configurable color model

- [ ] Drawable scaling
    - [ ] Nearest scaling
    - [ ] Bilinear scaling
    - [ ] (optional) Bicubic scaling
    - [ ] Configurable scaling

- [ ] Text drawables
    - [ ] `FreeType` integration
    - [ ] `HarfBuzz` shaping

- [ ] **Stabilization**
    - [ ] Stabilize the naming convention
    - [ ] Stabilize the standards
    - [ ] Stabilize the codebase
    - [ ] Stabilize the ABI
    - [ ] (optional) Stabilize the Build System

- [ ] Boolean drawable operations
    - [ ] Union operations
    - [ ] Subtract operations
    - [ ] Intersection operations
    - [ ] Masking operations

- [ ] Optimizations
    - [ ] Basic optimizations
    - [ ] Developer experience optimizations
    - [ ] Performance optimizations
    - [ ] (optional) Profiling

## License

Licensed under the Apache License 2.0.  
See [LICENSE](LICENSE).