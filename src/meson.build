lib_sources = files(
  'geometry/vec3.cpp',
  'geometry/ray.cpp',
  'object/object.cpp',
  'camera/image.cpp',
  'camera/camera.cpp',
  'camera/viewport.cpp',
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
