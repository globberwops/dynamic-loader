cmake_minimum_required(VERSION 3.24.0)

project(
  DynamicLoader
  VERSION 0.1.0
  LANGUAGES CXX)

option(DynamicLoader_BUILD_DOCS "Build docs" ${PROJECT_IS_TOP_LEVEL})
option(DynamicLoader_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(DynamicLoader_ENABLE_WARNINGS "Enable compiler warnings" ${PROJECT_IS_TOP_LEVEL})

# Compiler warnings
if(DynamicLoader_ENABLE_WARNINGS)
  if(MSVC)
    add_compile_options(
      /w14242
      /w14254
      /w14263
      /w14265
      /w14287
      /w14296
      /w14311
      /w14545
      /w14546
      /w14547
      /w14549
      /w14555
      /w14619
      /w14640
      /w14640
      /w14826
      /w14905
      /w14906
      /w14928
      /W4
      /we4289
      /WX)
  else()
    add_compile_options(
      -Wall
      -Wcast-align
      -Wconversion
      -Wdouble-promotion
      -Wduplicated-branches
      -Wduplicated-cond
      -Werror
      -Wextra
      -Wformat=2
      -Wlogical-op
      -Wmisleading-indentation
      -Wnon-virtual-dtor
      -Wnull-dereference
      -Wold-style-cast
      -Woverloaded-virtual
      -Wpedantic
      -Wshadow
      -Wsign-conversion
      -Wunused
      -Wuseless-cast)
  endif()
endif()

# Library
add_library(DynamicLoader INTERFACE)
add_library(DynamicLoader::DynamicLoader ALIAS DynamicLoader)
target_compile_features(DynamicLoader INTERFACE cxx_std_20)
target_link_libraries(DynamicLoader INTERFACE ${CMAKE_DL_LIBS})
target_sources(
  DynamicLoader
  INTERFACE FILE_SET
            HEADERS
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/DynamicLoader/DynamicLoader.hpp)
set_target_properties(DynamicLoader PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

# Installation
set(DynamicLoader_CONFIG_INSTALL_DIR
    lib/cmake/DynamicLoader
    CACHE PATH "Install path for package configs")
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/DynamicLoaderConfig.cmake.in ${PROJECT_BINARY_DIR}/DynamicLoaderConfig.cmake
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  INSTALL_DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR})

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/DynamicLoaderConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${PROJECT_BINARY_DIR}/DynamicLoaderConfig.cmake ${PROJECT_BINARY_DIR}/DynamicLoaderConfigVersion.cmake
        DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR})

install(
  TARGETS DynamicLoader
  EXPORT DynamicLoaderTargets
  FILE_SET HEADERS)

install(
  EXPORT DynamicLoaderTargets
  DESTINATION ${DynamicLoader_CONFIG_INSTALL_DIR}
  NAMESPACE DynamicLoader::)

export(
  EXPORT DynamicLoaderTargets
  NAMESPACE DynamicLoader::
  FILE ${PROJECT_BINARY_DIR}/DynamicLoaderTargets.cmake)

# Docs
if(DynamicLoader_BUILD_DOCS)
  add_subdirectory(docs)
endif()

# Tests
if(DynamicLoader_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
