lib_sources = files(
  'geometry/vec3.cpp',
  'geometry/ray.cpp',
  'geometry/sphere.cpp',
  'image.cpp',
  'camera.cpp',
  'renderer.cpp',
)

pathtracer_lib = static_library(
  'pathtracer',
  lib_sources,
  include_directories: project_inc,
)

pathtracer_dep = declare_dependency(
  link_with: pathtracer_lib,
  include_directories: project_inc,
)
