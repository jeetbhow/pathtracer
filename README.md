# pathtracer

A C++23 executable built with Meson.

Install a C++ compiler and Meson (plus Ninja for Meson), then run:

```sh
meson setup build
meson compile -C build
meson test -C build
./build/pathtracer
```

On Windows, run `build\pathtracer.exe`.

With Make installed, you can also build and run with `make run`, or build and
run the tests with `make test`. Both commands set up the build directory if needed.

The executable entry point is `app/main.cpp`. It sets up the camera and sphere,
calls the renderer, and writes `image.ppm` in the working directory.

Library sources live in `src/`, with public headers in `include/pathtracer/`.
Include them as `"pathtracer/camera.hpp"`, for example; library types and functions
use the `pathtracer` namespace. Add new source files to `src/meson.build`.

- `geometry/` groups `vec3`, `ray`, and `sphere` in both the public headers and
  library sources. They provide vector math, rays, sphere intersections, and
  surface normals.
- `camera` generates rays from camera settings and image dimensions.
- `image` stores pixels and writes PPM output.
- `renderer` handles the pixel loop, sphere shading, and sky background.

Google Test in `test/` links against the same pathtracer library as the executable.
Test files follow the library component names; register new files in
`test/meson.build`. Meson uses an installed Google Test or downloads the pinned
fallback in `subprojects/gtest.wrap` (internet access required). Tests use C++23.
