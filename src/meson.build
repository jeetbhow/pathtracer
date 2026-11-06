lib_sources = files(
  'app.cpp',
  'geometry.cpp',
  'image.cpp',
)

app_lib = static_library(
  'pathtracer',
  lib_sources,
  include_directories: project_inc,
)

app_dep = declare_dependency(
  link_with: app_lib,
  include_directories: project_inc,
)
