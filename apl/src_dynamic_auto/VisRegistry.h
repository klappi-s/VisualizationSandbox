#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <any>
#include <stdexcept>
#include <initializer_list>
#include <string_view>
#include <type_traits>


/////////////////////////////////////////////////////
// NOOOO STAAAAAATICS... minimize global ...
/////////////////////////////////////////////////////
class RegistryBase{

    public:

    virtual ~RegistryBase() = default;

    struct Entry {
        std::string name;
        std::any ptr_any;

        template <typename T>
        Entry(const std::string& entry_name, T* ptr)
            : name(entry_name), ptr_any(ptr) {}
    };
};



// Compile-time string literal wrapper (C++20 NTTP).
template <std::size_t N>
struct fixed_string {
    char data[N]{};
    constexpr fixed_string(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) data[i] = str[i];
    }
    constexpr std::string_view sv() const { return std::string_view{data, N - 1}; }
};
template <std::size_t N, std::size_t M>
constexpr bool operator==(const fixed_string<N>& a, const fixed_string<M>& b) {
    if constexpr (N != M) return false;
    for (std::size_t i = 0; i < N; ++i) if (a.data[i] != b.data[i]) return false;
    return true;
}

// Helper tag to pass fixed_string IDs as a type
// Usage: fs_tag<"name">{}
template <fixed_string Str>
struct fs_tag {};

// #define FIXED_STRING()





// A dynamic registry with compile-time name-only API via nested mappings
class RegistryDynamic : public RegistryBase {
private:
    std::unordered_map<std::string, void*> m_storage;  // string→void* storage

public:
    // Nested mapping: default unknown names to void
    template<fixed_string Name>
    struct NameToType { using type = void; };

    // Add/bind with compile-time name (SFINAE constrained by NameToType)
    template<fixed_string Name, typename U>
    auto Set(U& object) -> std::enable_if_t<
        std::is_same_v<typename NameToType<Name>::type, std::remove_const_t<U>>, void>
    {
        std::string key{ Name.sv() };
        m_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // Get with compile-time name (SFINAE ensures known name)
    template<fixed_string Name>
    auto Get() -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>,
        typename NameToType<Name>::type&
    >
    {
        using T = typename NameToType<Name>::type;
        std::string key{ Name.sv() };
        auto it = m_storage.find(key);
        if (it == m_storage.end() || it->second == nullptr) {
            throw std::runtime_error("Null or missing entry for ID: " + key);
        }
        return *static_cast<T*>(it->second);
    }

    template<fixed_string Name>
    auto Get() const -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>,
        const typename NameToType<Name>::type&
    >
    {
        using T = typename NameToType<Name>::type;
        std::string key{ Name.sv() };
        auto it = m_storage.find(key);
        if (it == m_storage.end() || it->second == nullptr) {
            throw std::runtime_error("Null or missing entry for ID: " + key);
        }
        return *static_cast<const T*>(it->second);
    }

    // Contains with compile-time name only (SFINAE ensures known name)
    template<fixed_string Name>
    auto Contains() const -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>, bool>
    {
        std::string key{ Name.sv() };
        return m_storage.find(key) != m_storage.end();
    }

    // Optional: Unset/remove binding by compile-time name
    template<fixed_string Name>
    auto Unset() -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>, bool>
    {
        std::string key{ Name.sv() };
        return m_storage.erase(key) > 0;
    }

    // Runtime string API
    template<typename T>
    void add_named(const std::string& name, T& object) {
        m_storage[name] = const_cast<void*>(static_cast<const void*>(&object));
    }

    template<typename T>
    T* get_named(const std::string& name) const {
        auto it = m_storage.find(name);
        return (it != m_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    bool contains_named(const std::string& name) const {
        return m_storage.find(name) != m_storage.end();
    }

    bool remove_named(const std::string& name) {
        return m_storage.erase(name) > 0;
    }
};

// Macro to register name→type for this handler (must be at namespace scope)
#define REGDYN_REGISTER_NAME_TYPE(name_lit, ...) \
    template<> struct RegistryDynamic::NameToType<fixed_string{name_lit}> { using type = __VA_ARGS__; }






