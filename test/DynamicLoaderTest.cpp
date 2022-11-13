#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>

#include "DynamicLoader/DynamicLoader.hpp"
#include "test/Config.hpp"

namespace {
const auto test_lib_path = std::filesystem::path{kProjectBinaryDir} / "test" / "libDynamicLoaderTestLib.so";
}  // namespace

TEST_CASE("DynamicLoaderTest, LoadLib") {
  CHECK_NOTHROW(DynamicLoader{test_lib_path});
}

TEST_CASE("DynamicLoaderTest, LoadLibThrow") {
  CHECK_THROWS_AS(DynamicLoader{"Throw"}, std::runtime_error);
}

TEST_CASE("DynamicLoaderTest, Lookup") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto func = dynamic_loader.Lookup<void()>("DynamicLoaderTest");
  func();
}

TEST_CASE("DynamicLoaderTest, LookupThrow") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  CHECK_THROWS_AS(dynamic_loader.Lookup<void()>("Throw"), std::runtime_error);
}

TEST_CASE("DynamicLoaderTest, LookupParam") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto func = dynamic_loader.Lookup<void(int)>("DynamicLoaderTestParam");
  CHECK_NOTHROW(func(42));
}

TEST_CASE("DynamicLoaderTest, LookupReturn") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto func = dynamic_loader.Lookup<int()>("DynamicLoaderTestReturn");
  CHECK(func() == 42);
}

TEST_CASE("DynamicLoaderTest, LookupParamReturn") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto func = dynamic_loader.Lookup<int(int)>("DynamicLoaderTestParamReturn");
  CHECK(func(42) == 42);
}

TEST_CASE("DynamicLoaderTest, Mangle") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto mangled = dynamic_loader.Mangle("DynamicLoaderTestMangled");
  CHECK(mangled == "_Z24DynamicLoaderTestMangledi");
}

TEST_CASE("DynamicLoaderTest, MangleThrow") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  CHECK_THROWS_AS(dynamic_loader.Mangle("Throw"), std::runtime_error);
}

TEST_CASE("DynamicLoaderTest, MangleAndLookup") {
  auto dynamic_loader = DynamicLoader{test_lib_path};

  auto mangled = dynamic_loader.Mangle("DynamicLoaderTestMangled");
  auto func = dynamic_loader.Lookup<int(int)>(mangled);
  CHECK(func(42) == 42);
}
