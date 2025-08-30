#include <iostream>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <array>
#include <string_view>

// =================================================================================
// FixedString for Compile-Time String Tags
// =================================================================================

template<size_t N>
struct FixedString {
    std::array<char, N> data{};
    
    constexpr FixedString(const char* str) {
        for (size_t i = 0; i < N; ++i) data[i] = str[i];
    }
    
    constexpr std::string_view to_view() const {
        return {data.data(), N-1}; // Exclude null terminator
    }
    
    constexpr bool operator==(const FixedString& other) const {
        return data == other.data;
    }
    
    constexpr size_t size() const { return N - 1; }
};

// Deduction guide for FixedString
template<size_t N>
FixedString(const char (&)[N]) -> FixedString<N>;









// =================================================================================
// Compile-time name → type mapping for constrained overloads
// =================================================================================

// Register compile-time name→type mappings used in this demo BEFORE use
template<FixedString Name>
struct NameToType { using type = void; };

#define REGISTER_NAME_TYPE(name_lit, T) \
    template<> struct NameToType<FixedString{name_lit}> { using type = T; }


// =================================================================================
// Registry Handler Supporting Name-Only and Runtime String Access
// =================================================================================




// =================================================================================
// Registry Implementation
// =================================================================================


class RegistryHandler {
private:
    std::unordered_map<std::string, void*> m_string_storage;  // string-based

public:
    // ----- Add with compile-time name only (SFINAE constrained)
    template<FixedString Name, typename U>
    auto add(U& object) -> std::enable_if_t<
        std::is_same_v<typename NameToType<Name>::type, std::remove_const_t<U>>, void>
    {
        std::string key{ Name.to_view() };
        m_string_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // ----- Get with compile-time name only (SFINAE ensures known name)
    template<FixedString Name>
    auto get() -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>,
        typename NameToType<Name>::type*
    >
    {
        using T = typename NameToType<Name>::type;
        std::string key{ Name.to_view() };
        auto it = m_string_storage.find(key);
        return (it != m_string_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    // ----- Contains with compile-time name only (SFINAE ensures known name)
    template<FixedString Name>
    auto contains() const -> std::enable_if_t<
        !std::is_same_v<typename NameToType<Name>::type, void>, bool>
    {
        std::string key{ Name.to_view() };
        return m_string_storage.find(key) != m_string_storage.end();
    }

    // ----- Runtime string API
    template<typename T>
    void add_named(const std::string& name, T& object) {
        m_string_storage[name] = const_cast<void*>(static_cast<const void*>(&object));
    }

    template<typename T>
    T* get_named(const std::string& name) {
        auto it = m_string_storage.find(name);
        return (it != m_string_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    bool contains_named(const std::string& name) const {
        return m_string_storage.find(name) != m_string_storage.end();
    }
};






// =================================================================================
// Demo
// =================================================================================




REGISTER_NAME_TYPE("player_name", std::string);
REGISTER_NAME_TYPE("score", int);
REGISTER_NAME_TYPE("is_active", bool);
REGISTER_NAME_TYPE("gravity", double);
REGISTER_NAME_TYPE("health", float);


int main() {
    std::string player_name = "Zrich_Champion"; // Zürich_Champion (UTF-8)
    int score = 2025;
    bool is_active = true;
    const double gravity = 9.81;
    float health = 100.0f;

    RegistryHandler handler;

    std::cout << "=== Name-Only Constrained API Demo (bdemo_three) ===\n";

    // Add using compile-time names (no explicit types or tag structs)
    // handler.add<NAME("player_name")>(player_name);
    // handler.add<NAME("score")>(score);
    // handler.add<NAME("is_active")>(is_active);
    // handler.add<NAME("gravity")>(gravity);
    // handler.add<NAME("health")>(health);

    handler.add<"player_name">(player_name);
    handler.add<"score">(score);
    handler.add<"is_active">(is_active);
    handler.add<"gravity">(gravity);
    handler.add<"health">(health);

    // Retrieve using compile-time names
    std::cout << "Player:  " << *handler.get<"player_name">() << '\n';
    std::cout << "Score:   " << *handler.get<"score">() << '\n';
    std::cout << "Active:  " << std::boolalpha << *handler.get<"is_active">() << '\n';
    std::cout << "Gravity: " << *handler.get<"gravity">() << '\n';
    std::cout << "Health:  " << *handler.get<"health">() << '\n';

    // Contains checks
    std::cout << "\nContains checks (compile-time names):\n";
    std::cout << "  player_name: " << std::boolalpha << handler.contains<"player_name">() << '\n';
    // Note: calling contains<NAME("doesnt_exist")>() would be ill-formed (no mapping).

    // Runtime string-based access (less type-safe but flexible)
    handler.add_named("runtime_tag", health);
    if (auto* runtime_health = handler.get_named<float>("runtime_tag")) {
        std::cout << "\nRuntime Health: " << *runtime_health << '\n';
    }
    std::cout << "Contains runtime_tag: " << std::boolalpha << handler.contains_named("runtime_tag") << '\n';

    // Demonstrate modification via name-only API
    *handler.get<"score">() = 9001;
    *handler.get<"health">() = 85.5f;
    std::cout << "\nModified score: " << score << '\n';
    std::cout << "Modified health: " << health << '\n';

    return 0;
}
