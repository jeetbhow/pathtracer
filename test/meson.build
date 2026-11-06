gtest_dep = dependency('gtest', main: true, fallback: ['gtest', 'gtest_main_dep'])

test_sources = files(
  'camera_test.cpp',
  'geometry_test.cpp',
  'image_test.cpp',
)

unit_tests = executable(
  'unit_tests',
  test_sources,
  dependencies: [pathtracer_dep, gtest_dep],
  override_options: ['cpp_std=c++23'],
)

test('unit', unit_tests)
