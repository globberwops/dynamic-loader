cmake_minimum_required(VERSION 3.21.0)

project(
  DynamicLoader
  VERSION 1.0.0
  LANGUAGES CXX)

option(DynamicLoader_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})

add_library(DynamicLoader INTERFACE)
add_library(DynamicLoader::DynamicLoader ALIAS DynamicLoader)
target_compile_features(DynamicLoader INTERFACE cxx_std_17)
target_include_directories(
  DynamicLoader INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:include>)
target_link_libraries(DynamicLoader INTERFACE ${CMAKE_DL_LIBS})

set(DynamicLoader_CONFIG_INSTALL_DIR lib/cmake/DynamicLoader)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/DynamicLoaderConfig.cmake.in
  ${PROJECT_BINARY_DIR}/DynamicLoaderConfig.cmake
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  INSTALL_DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR})

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/DynamicLoaderConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${PROJECT_BINARY_DIR}/DynamicLoaderConfig.cmake
              ${PROJECT_BINARY_DIR}/DynamicLoaderConfigVersion.cmake
        DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR})

install(TARGETS DynamicLoader EXPORT DynamicLoaderTargets)

install(
  EXPORT DynamicLoaderTargets
  DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR}
  NAMESPACE DynamicLoader::)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ TYPE INCLUDE)

export(
  EXPORT DynamicLoaderTargets
  NAMESPACE DynamicLoader::
  FILE ${PROJECT_BINARY_DIR}/DynamicLoaderTargets.cmake)

if(DynamicLoader_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
