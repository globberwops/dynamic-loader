#pragma once

#include <dlfcn.h>
#include <link.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

/// \brief DynamicLoader namespace
namespace dl
{

/// \brief DynamicLoader options
/// \see <a href="https://linux.die.net/man/3/dlopen">dlopen(3) - Linux man page</a>
struct Options
{
    bool rtld_deepbind{};  ///< Place the lookup scope of the symbols in this library ahead of the global scope.
    bool rtld_global{};    ///< Make symbols defined by this library available to subsequently loaded libraries.
    bool rtld_lazy{true};  ///< Only resolve symbols as the code that references them is executed.
    bool rtld_local{true}; ///< Do not make symbols defined by this library available to subsequently loaded libraries.
    bool rtld_nodelete{};  ///< Do not unload the library during dlclose().
    bool rtld_noload{};    ///< Do not load the library. Can be used to test if library is already resident.
    bool rtld_now{};       ///< Resolve all undefined symbols in the library before dlopen() returns.
};

/// \brief Load a dynamic library
/// \tparam TOptions Options for dynamic loading
template <Options TOptions = Options{}>
class DynamicLoader
{
    struct Deleter
    {
        void operator()(void *ptr)
        {
            dlclose(ptr);
        }
    };

    std::unique_ptr<void, Deleter> m_handle;

  public:
    /// \brief Load a dynamic library
    /// \param[in] path The path of the libary
    /// \throw std::runtime_error
    explicit DynamicLoader(const std::filesystem::path &path)
    {
        constexpr int flags{(TOptions.rtld_deepbind ? RTLD_DEEPBIND : 0) | //
                            (TOptions.rtld_global ? RTLD_GLOBAL : 0) |     //
                            (TOptions.rtld_lazy ? RTLD_LAZY : 0) |         //
                            // (TOptions.rtld_local ? RTLD_LOCAL : 0) |    //
                            (TOptions.rtld_nodelete ? RTLD_NODELETE : 0) | //
                            (TOptions.rtld_noload ? RTLD_NOLOAD : 0) |     //
                            (TOptions.rtld_now ? RTLD_NOW : 0)};

        auto *ptr = dlopen(path.c_str(), flags);

        if (auto *error = dlerror(); error != nullptr)
        {
            throw std::runtime_error{error};
        }

        m_handle.reset(ptr);
    }

    /// \brief Lookup a symbol in the library
    /// \tparam Signature The function signature of the symbol
    /// \param[in] symbol The name of the symbol
    /// \return The symbol wrapped in an std::function
    /// \throw std::runtime_error
    template <typename Signature>
    auto Lookup(const std::string &symbol) -> std::function<Signature>
    {
        auto *ptr = dlsym(m_handle.get(), symbol.c_str());

        if (auto *error = dlerror(); error != nullptr)
        {
            throw std::runtime_error{error};
        }

        return {reinterpret_cast<Signature *>(ptr)};
    }

    /// \brief Mangle the name of a symbol
    /// \param[in] symbol The name of the symbol
    /// \return The mangled name of the symbol
    /// \throw std::runtime_error
    auto Mangle(const std::string &symbol) -> std::string
    {
        struct link_map *link_map{};
        dlinfo(m_handle.get(), RTLD_DI_LINKMAP, &link_map);

        if (auto *error = dlerror(); error != nullptr)
        {
            throw std::runtime_error{error};
        }

        Elf64_Sym *symbol_table{};
        char *string_table{};
        std::uint64_t symbol_entries{};

        for (auto *section = link_map->l_ld; section->d_tag != DT_NULL; ++section)
        {
            switch (section->d_tag)
            {
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

        auto size = static_cast<std::uint64_t>(string_table - reinterpret_cast<char *>(symbol_table));
        for (auto k = 0U; k < size / symbol_entries; ++k)
        {
            auto *sym = &symbol_table[k];
            if (ELF64_ST_TYPE(symbol_table[k].st_info) == STT_FUNC)
            {
                auto *symbol_name = &string_table[sym->st_name];
                if (auto *result = strstr(symbol_name, symbol.c_str()); result != nullptr)
                {
                    return {symbol_name};
                }
            }
        }

        throw std::runtime_error{symbol + ": cannot mangle symbol name: No such symbol"};
    }
};

} // namespace dl
