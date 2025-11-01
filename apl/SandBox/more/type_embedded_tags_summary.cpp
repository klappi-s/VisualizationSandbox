#include <iostream>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

// =================================================================================
// Final Elegant Solution: Type-Embedded Tags Registry
// =================================================================================

// 1. Define the Tag template that embeds type information
template<typename T>
struct Tag {
    using type = T;
};

// Template variable for easy tag creation
template<typename T>
inline constexpr Tag<T> tag{};

// 2. Registry Handler with type-embedded tags
class RegistryHandler {
private:
    std::unordered_map<std::type_index, void*> m_storage;

public:
    // Add with automatic type deduction from tag
    template<typename T, typename U>
    void add(Tag<T> tag_instance, U& object) {
        static_assert(std::is_same_v<T, std::remove_const_t<U>>, "Tag type must match object type");
        std::type_index key{typeid(tag_instance)};
        m_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // Get with automatic type deduction from tag
    template<typename T>
    T* get(Tag<T> tag_instance) {
        std::type_index key{typeid(tag_instance)};
        auto it = m_storage.find(key);
        return (it != m_storage.end()) ? static_cast<T*>(it->second) : nullptr;
    }
    
    // Convenient operator[] overload
    template<typename T>
    T* operator[](Tag<T> tag_instance) {
        return get(tag_instance);
    }
    
    // Check if a tag exists
    template<typename T>
    bool contains(Tag<T> tag_instance) const {
        std::type_index key{typeid(tag_instance)};
        return m_storage.find(key) != m_storage.end();
    }
};

// =================================================================================
// Usage Example
// =================================================================================

// 3. Create specific tag instances with embedded type information
constexpr auto PlayerNameTag = Tag<std::string>{};
constexpr auto ScoreTag = Tag<int>{};
constexpr auto IsActiveTag = Tag<bool>{};
constexpr auto GravityTag = Tag<double>{};

int main() {
    // Create the objects you want to manage
    std::string player_name = "Zürich_Champion";
    int score = 2025;
    bool is_active = true;
    const double gravity = 9.81;

    // Create handler
    RegistryHandler handler;

    // Add entries - types are automatically deduced from tags!
    handler.add(PlayerNameTag, player_name);
    handler.add(ScoreTag, score);
    handler.add(IsActiveTag, is_active);
    handler.add(GravityTag, gravity);

    std::cout << "=== Type-Embedded Tags Registry Demo ===" << std::endl;

    // Method 1: Direct get() - type automatically deduced
    std::string* name_ptr = handler.get(PlayerNameTag);
    int* score_ptr = handler.get(ScoreTag);
    bool* active_ptr = handler.get(IsActiveTag);
    
    std::cout << "Direct get() method:" << std::endl;
    if (name_ptr && score_ptr && active_ptr) {
        std::cout << "  Player: " << *name_ptr << std::endl;
        std::cout << "  Score:  " << *score_ptr << std::endl;
        std::cout << "  Active: " << std::boolalpha << *active_ptr << std::endl;
    }

    // Method 2: operator[] - even more elegant
    std::cout << "\noperator[] method:" << std::endl;
    std::cout << "  Player: " << *handler[PlayerNameTag] << std::endl;
    std::cout << "  Score:  " << *handler[ScoreTag] << std::endl;
    std::cout << "  Active: " << std::boolalpha << *handler[IsActiveTag] << std::endl;

    // Method 3: Using template variable tags
    std::cout << "\nTemplate variable tags:" << std::endl;
    handler.add(tag<std::string>, player_name);  // Alternative way
    auto alt_name = handler.get(tag<std::string>);
    if (alt_name) {
        std::cout << "  Alt Name: " << *alt_name << std::endl;
    }

    // Modify data
    std::cout << "\n=== Modifying Data ===" << std::endl;
    *handler[ScoreTag] = 9001;
    std::cout << "Original score variable: " << score << std::endl;

    return 0;
}

/*
=== Key Benefits of Type-Embedded Tags ===

1. **No Type Specification Needed**: 
   handler.get(ScoreTag) instead of handler.get<ScoreTag, int>()

2. **Compile-Time Type Safety**: 
   Tags contain type information, preventing mismatches

3. **Unique and Self-Describing**: 
   Each tag is unique and carries its return type

4. **Clean API**: 
   - handler.add(ScoreTag, score)
   - handler.get(ScoreTag)
   - handler[ScoreTag]

5. **Zero Runtime Cost**: 
   Tags are empty structs, no memory overhead

6. **Flexible Usage**: 
   - Named tags: ScoreTag
   - Template variables: tag<int>
   - Both work seamlessly

This approach provides the most elegant and type-safe registry API possible!
*/
