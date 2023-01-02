include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
CPMAddPackage("gh:doctest/doctest@2.4.9")

configure_file(Config.hpp.in Config.hpp @ONLY)

add_library(DynamicLoaderTestLib SHARED)
target_sources(DynamicLoaderTestLib PRIVATE DynamicLoaderTestLib.cpp)
target_compile_features(DynamicLoaderTestLib PRIVATE cxx_std_20)

add_executable(DynamicLoaderTest)
target_sources(DynamicLoaderTest PRIVATE DynamicLoaderTest.cpp)
target_include_directories(DynamicLoaderTest PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(DynamicLoaderTest PRIVATE DynamicLoader::DynamicLoader doctest::doctest)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(DynamicLoaderTest)
