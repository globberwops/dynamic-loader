#pragma once

#include <dlfcn.h>
#include <link.h>

#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

class DynamicLoader {
  struct Deleter {
    void operator()(void *ptr) { dlclose(ptr); }
  };

  std::unique_ptr<void, Deleter> handle;

 public:
  DynamicLoader(const std::filesystem::path &path, int flags = RTLD_NOW) {
    auto *ptr = dlopen(path.c_str(), flags);

    if (auto *error = dlerror(); error != nullptr) {
      throw std::runtime_error{error};
    }

    handle.reset(ptr);
  }

  template <typename Signature>
  auto Lookup(const std::string &symbol) -> std::function<Signature> {
    auto *ptr = dlsym(handle.get(), symbol.c_str());

    if (auto *error = dlerror(); error != nullptr) {
      throw std::runtime_error{error};
    }

    return {reinterpret_cast<Signature *>(ptr)};
  }

  auto Mangle(const std::string &symbol) -> std::string {
    struct link_map *link_map{};
    dlinfo(handle.get(), RTLD_DI_LINKMAP, &link_map);

    Elf64_Sym *symbol_table{};
    char *string_table{};
    int symbol_entries{};

    for (auto section = link_map->l_ld; section->d_tag != DT_NULL; ++section) {
      switch (section->d_tag) {
        case DT_SYMTAB:
          symbol_table = reinterpret_cast<Elf64_Sym *>(section->d_un.d_ptr);
          break;
        case DT_STRTAB:
          string_table = reinterpret_cast<char *>(section->d_un.d_ptr);
          break;
        case DT_SYMENT:
          symbol_entries = section->d_un.d_val;
          break;
        default:
          break;
      }
    }

    int size = string_table - reinterpret_cast<char *>(symbol_table);
    for (int k = 0; k < size / symbol_entries; ++k) {
      auto sym = &symbol_table[k];
      if (ELF64_ST_TYPE(symbol_table[k].st_info) == STT_FUNC) {
        auto *symbol_name = &string_table[sym->st_name];
        if (auto *result = strstr(symbol_name, symbol.c_str()); result != nullptr) {
          return {symbol_name};
        }
      }
    }

    throw std::runtime_error{"Symbol not found: " + symbol};
  }
};
