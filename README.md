# DynamicLoader

![CI](https://github.com/globberwops/dynamic-loader/actions/workflows/ci.yml/badge.svg)

DynamicLoader is a header-only library providing a modern C++ interface to dynamically loaded libraries ("shared objects").
It is built on top of the Linux [dlopen()](https://man7.org/linux/man-pages/man3/dlopen.3.html) API.

Try DynamicLoader in [Compiler Explorer](https://godbolt.org/z/KvGxYhErh).

## Features

* Load shared objects (`*.so`)
* Mangle function names (no `extern "C"` needed)
* Lookup functions and get an `std::function`
* Header-only
* Throws exceptions
* Clean warning-free codebase
* Small self-contained codebase

## Limitations

* Linux only
* Does not support function overloads in the loaded library

## Example

```C++
#include <DynamicLoader/DynamicLoader.hpp>

try {
  auto dynamic_loader = dl::DynamicLoader{"<path/to/lib>"};
  auto mangled_name = dynamic_loader.Mangle("Square");
  auto function = dynamic_loader.Lookup<int(int)>(mangled_name);
  assert(function(2) == 4);
} catch (const std::exception& ex) {
  std::cout << ex.what() << '\n';
}
```

## Configuration Options

| Name                               | Description                      | Default                   |
|------------------------------------|----------------------------------|---------------------------|
| `DynamicLoader_BUILD_DOCS`         | Build the documentation          | `$PROJECT_IS_TOP_LEVEL`   |
| `DynamicLoader_BUILD_TESTS`        | Build the tests                  | `$PROJECT_IS_TOP_LEVEL`   |
| `DynamicLoader_ENABLE_WARNINGS`    | Enable compiler warnings         | `$PROJECT_IS_TOP_LEVEL`   |
| `DynamicLoader_CONFIG_INSTALL_DIR` | Install path for package configs | `lib/cmake/DynamicLoader` |

## Building

DynamicLoader uses the [CMake](https://cmake.org/) build system, and [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) for its (optional) dependencies.
You need CMake >= 3.24.0, Clang >= 12, or GCC >= 9 to build the library.

```Bash
cmake -B build [-GNinja] [-DCMAKE_BUILD_TYPE=Release]
cmake --build build [--config Release]
cmake --install build [--config Release] [--prefix build/prefix]
```

## Testing

DynamicLoader uses [CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) and [doctest](https://github.com/doctest/doctest) for its unit tests.

```Bash
cd build
ctest [-C Release]
```

## Documentation

DynamicLoader uses [Doxygen](https://www.doxygen.nl/) and [Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css) for its documentation.

```Bash
cmake --build build --target DynamicLoaderDocs
```

Please find the HTML documentation in `build/docs/html`

## Usage

DynamicLoader can be integrated into your project in several ways.

### CMake FetchContent

```CMake
include(FetchContent)
FetchContent_Declare(
  dynamic-loader
  GIT_REPOSITORY https://github.com/globberwops/dynamic-loader.git
  GIT_TAG <tag>
)
FetchContent_MakeAvailable(dynamic-loader)
...
target_link_libraries(<your-target> PRIVATE DynamicLoader::DynamicLoader)
```

### Git Submodule

```Bash
git submodule add https://github.com/globberwops/dynamic-loader.git third_party/dynamic-loader
cd third_party/dynamic-loader
git checkout <tag>
```

```CMake
add_subdirectory(third_party/dynamic-loader EXCLUDE_FROM_ALL)
...
target_link_libraries(<your-target> PRIVATE DynamicLoader::DynamicLoader)
```

### Vcpkg

```Bash
vcpkg install globberwops-dynamic-loader
```

```CMake
find_package(DynamicLoader CONFIG REQUIRED)
...
target_link_libraries(<your-target> PRIVATE DynamicLoader::DynamicLoader)
```
