lib_sources = files(
  'geometry.cpp',
  'image.cpp',
  'scene.cpp',
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
