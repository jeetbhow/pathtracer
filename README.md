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

Put library sources in `src/`, public headers in `include/`, and add source files
to the build file in `src/`. The root `main.cpp` links against the same library.

Google Test in `test/` links against the pathtracer library. Meson uses an installed Google Test or downloads the pinned fallback in `subprojects/gtest.wrap` (internet access required). Tests use C++23.
