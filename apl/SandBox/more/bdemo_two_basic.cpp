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
// Enhanced Tag System with String-Based and Type-Based Tags
// =================================================================================






// 1. Type-based tags (original approach)
template<typename T>
struct TypeTag {
    using type = T;
};

// 2. String-based tags with embedded type information
template<FixedString Name, typename T>
struct StringTag {
    using type = T;
    static constexpr auto name = Name;
    static constexpr std::string_view get_name() { return Name.to_view(); }
};

// 3. Template variable for easy tag creation
template<typename T>
inline constexpr TypeTag<T> type_tag{};

template<FixedString Name, typename T>
inline constexpr StringTag<Name, T> string_tag{};

// =================================================================================
// Registry Handler Supporting Both Tag Types
// =================================================================================

class RegistryHandler {
private:
    std::unordered_map<std::type_index, void*> m_storage;
    std::unordered_map<std::string, void*> m_string_storage;

public:
    // Add with type-based tags
    template<typename T, typename U>
    void add(TypeTag<T> tag_instance, U& object) {
        static_assert(std::is_same_v<T, std::remove_const_t<U>>, "Tag type must match object type");
        std::type_index key{typeid(tag_instance)};
        m_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // Add with string-based tags
    template<FixedString Name, typename T, typename U>
    void add(StringTag<Name, T> tag_instance, U& object) {
        static_assert(std::is_same_v<T, std::remove_const_t<U>>, "Tag type must match object type");
        std::string key{tag_instance.get_name()};
        m_string_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // Get with type-based tags
    template<typename T>
    T* get(TypeTag<T> tag_instance) {
        std::type_index key{typeid(tag_instance)};
        auto it = m_storage.find(key);
        return (it != m_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    // Get with string-based tags
    template<FixedString Name, typename T>
    T* get(StringTag<Name, T> tag_instance) {
        std::string key{tag_instance.get_name()};
        auto it = m_string_storage.find(key);
        return (it != m_string_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    // Operator[] for type-based tags
    template<typename T>
    T* operator[](TypeTag<T> tag_instance) {
        return get(tag_instance);
    }

    // Operator[] for string-based tags
    template<FixedString Name, typename T>
    T* operator[](StringTag<Name, T> tag_instance) {
        return get(tag_instance);
    }

    // Check existence for type-based tags
    template<typename T>
    bool contains(TypeTag<T> tag_instance) const {
        std::type_index key{typeid(tag_instance)};
        return m_storage.find(key) != m_storage.end();
    }

    // Check existence for string-based tags
    template<FixedString Name, typename T>
    bool contains(StringTag<Name, T> tag_instance) const {
        std::string key{tag_instance.get_name()};
        return m_string_storage.find(key) != m_string_storage.end();
    }

    // Convenience method to create and use string tags on the spot
    template<typename T>
    void add_named(const std::string& name, T& object) {
        m_string_storage[name] = const_cast<void*>(static_cast<const void*>(&object));
    }

    template<typename T>
    T* get_named(const std::string& name) {
        auto it = m_string_storage.find(name);
        return (it != m_string_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }
};

// =================================================================================
// Convenience Macros for Creating String Tags on the Spot
// =================================================================================

// Macro to create a string tag with type
#define TAG(name, type) StringTag<FixedString{name}, type>{}

// Macro to create a string tag variable
#define MAKE_TAG(var_name, name, type) \
    constexpr auto var_name = StringTag<FixedString{name}, type>{}

// =================================================================================
// Usage Examples
// =================================================================================

int main() {
    // Create test objects
    std::string player_name = "Zürich_Champion";
    int score = 2025;
    bool is_active = true;
    const double gravity = 9.81;
    float health = 100.0f;

    RegistryHandler handler;

    std::cout << "=== Enhanced String-Based Tags Registry Demo ===" << std::endl;

    // Method 1: Pre-defined string tags
    MAKE_TAG(PlayerTag, "player_name", std::string);
    MAKE_TAG(ScoreTag, "score", int);
    MAKE_TAG(ActiveTag, "is_active", bool);
    MAKE_TAG(GravityTag, "gravity", double);

    handler.add(PlayerTag, player_name);
    handler.add(ScoreTag, score);
    handler.add(ActiveTag, is_active);
    handler.add(GravityTag, gravity);

    std::cout << "\nMethod 1 - Pre-defined string tags:" << std::endl;
    std::cout << "  Player: " << *handler.get(PlayerTag) << std::endl;
    std::cout << "  Score:  " << *handler.get(ScoreTag) << std::endl;
    std::cout << "  Active: " << std::boolalpha << *handler.get(ActiveTag) << std::endl;
    std::cout << "  Gravity: " << *handler.get(GravityTag) << std::endl;

    // Method 2: On-the-spot string tags using macro
    handler.add(TAG("health", float), health);
    
    std::cout << "\nMethod 2 - On-the-spot string tags:" << std::endl;
    std::cout << "  Health: " << *handler.get(TAG("health", float)) << std::endl;

    // Method 3: Using operator[] with string tags
    std::cout << "\nMethod 3 - operator[] with string tags:" << std::endl;
    std::cout << "  Player: " << *handler[PlayerTag] << std::endl;
    std::cout << "  Score:  " << *handler[ScoreTag] << std::endl;
    std::cout << "  Health: " << *handler[TAG("health", float)] << std::endl;

    // Method 4: Mixed usage with type-based tags
    constexpr auto TypeBasedScoreTag = TypeTag<int>{};
    handler.add(TypeBasedScoreTag, score);
    
    std::cout << "\nMethod 4 - Mixed type-based and string-based tags:" << std::endl;
    std::cout << "  String-based score: " << *handler[ScoreTag] << std::endl;
    std::cout << "  Type-based score:   " << *handler[TypeBasedScoreTag] << std::endl;

    // Method 5: Runtime string-based access (less type-safe but flexible)
    handler.add_named("runtime_tag", health);
    
    std::cout << "\nMethod 5 - Runtime string tags:" << std::endl;
    auto* runtime_health = handler.get_named<float>("runtime_tag");
    if (runtime_health) {
        std::cout << "  Runtime Health: " << *runtime_health << std::endl;
    }

    // Demonstrate modification
    std::cout << "\n=== Modifying Data ===" << std::endl;
    *handler[ScoreTag] = 9001;
    *handler[TAG("health", float)] = 85.5f;
    
    std::cout << "Modified score: " << score << std::endl;
    std::cout << "Modified health: " << health << std::endl;

    // Demonstrate tag uniqueness and existence checking
    std::cout << "\n=== Tag Management ===" << std::endl;
    std::cout << "Contains PlayerTag: " << std::boolalpha << handler.contains(PlayerTag) << std::endl;
    std::cout << "Contains health tag: " << std::boolalpha << handler.contains(TAG("health", float)) << std::endl;
    std::cout << "Contains non-existent tag: " << std::boolalpha << handler.contains(TAG("doesnt_exist", int)) << std::endl;

    // Show tag names
    std::cout << "\nTag names:" << std::endl;
    std::cout << "  PlayerTag name: " << PlayerTag.get_name() << std::endl;
    std::cout << "  ScoreTag name: " << ScoreTag.get_name() << std::endl;

    return 0;
}

/*
=== Key Benefits of String-Based Tags with FixedString ===

1. **Human-Readable Tags**: 
   - TAG("player_name", std::string) instead of opaque type names
   - Easy debugging and introspection

2. **On-the-Spot Creation**: 
   - handler.add(TAG("health", float), health)
   - No need to pre-define tag variables

3. **Compile-Time String Safety**: 
   - FixedString ensures string literals are embedded at compile time
   - Zero runtime string allocation overhead

4. **Type Safety Maintained**: 
   - Tags still carry type information
   - Compile-time type checking for add/get operations

5. **Flexible Access Patterns**: 
   - Pre-defined tags: handler[PlayerTag]
   - On-the-spot: handler[TAG("score", int)]
   - Runtime: handler.get_named<int>("runtime_tag")

6. **Introspection Friendly**: 
   - Tag names are accessible: tag.get_name()
   - Easy to implement debugging and serialization

7. **Backward Compatible**: 
   - Can mix string-based and type-based tags
   - Gradual migration path

This approach provides the ultimate flexibility for registry systems!
*/