project(
  'pathtracer',
  'cpp',
  version: '0.0.1',
  default_options: [
    'cpp_std=c++23',
    'warning_level=2',
  ],
)

project_inc = include_directories('include')

subdir('src')
subdir('test')

executable(
  'pathtracer',
  'main.cpp',
  dependencies: pathtracer_dep,
  install: true,
)
