# DynamicLoader

DynamicLoader is an open-source header-only library providing a modern interface to dynamically loaded libraries ("shared objects").

It provides a modern C++ interface built on top of the Linux [dlopen()](https://man7.org/linux/man-pages/man3/dlopen.3.html) API.

## Features

* Load shared objects (`*.so`)
* Mangle function names (no `extern "C"` needed)
* Lookup functions and get an `std::function`
* Throw exceptions

## Limitations

* Linux only
* Does not support function overloads in the loaded library

## Usage

```C++
try {
  auto dynamic_loader = DynamicLoader{"<path/to/lib>"};
  auto mangled_name = dynamic_loader.Mangle("Square");
  auto function = dynamic_loader.Lookup<int(int)>(mangled_name);
  assert(func(2) == 4);
} catch (const std::exception& ex) {
  std::cout << ex.what() << '\n';
}
```

Uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) and [doctest](https://github.com/doctest/doctest).
