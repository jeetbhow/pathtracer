gtest_dep = dependency('gtest', main: true, fallback: ['gtest', 'gtest_main_dep'])

test_sources = files(
  'camera_test.cpp',
  'vec3_test.cpp',
  'ray_test.cpp',
  'sphere_test.cpp',
  'plane_test.cpp',
  'material_test.cpp',
  'image_test.cpp',
  'renderer_test.cpp',
)

unit_tests = executable(
  'unit_tests',
  test_sources,
  dependencies: [pathtracer_dep, gtest_dep],
)

test('unit', unit_tests)
