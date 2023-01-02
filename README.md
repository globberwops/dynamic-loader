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

## Usage

```C++
try {
  auto dynamic_loader = DynamicLoader{"<path/to/lib>"};
  auto mangled_name = dynamic_loader.Mangle("Square");
  auto function = dynamic_loader.Lookup<int(int)>(mangled_name);
  assert(function(2) == 4);
} catch (const std::exception& ex) {
  std::cout << ex.what() << '\n';
}
```

## Configuration options

| Name                          | Description                                                           |
|-------------------------------|-----------------------------------------------------------------------|
| DynamicLoader_BUILD_DOCS      | Build the documentation (requires [Doxygen](https://www.doxygen.nl/)) |
| DynamicLoader_BUILD_TESTS     | Build the tests                                                       |
| DynamicLoader_ENABLE_WARNINGS | Enable compiler warnings                                              |

Uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake), [doctest](https://github.com/doctest/doctest), and [Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css).
