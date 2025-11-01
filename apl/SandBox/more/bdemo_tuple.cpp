#include <iostream>
#include <tuple>
#include <string>
#include <utility>
#include <type_traits>
#include <memory>
#include <functional>
#include <any>
#include <typeindex>
#include <unordered_map>

// =================================================================================
// PART 1: The Core Immutable, Compile-Time Safe Registry
// =================================================================================

// --- Metaprogramming helpers to find a pair by its ID tag in a tuple ---
namespace detail {
    template<typename IdTag, typename Tuple, std::size_t I = 0>
    struct find_by_id_tag_impl {
        static constexpr auto& run(Tuple& t) {
            using CurrentPair = std::tuple_element_t<I, Tuple>;
            using CurrentIdTag = typename CurrentPair::first_type;

            if constexpr (std::is_same_v<IdTag, CurrentIdTag>) {
                return std::get<I>(t);
            } else if constexpr (I + 1 < std::tuple_size_v<Tuple>) {
                return find_by_id_tag_impl<IdTag, Tuple, I + 1>::run(t);
            } else {
                static_assert(!std::is_same_v<IdTag, IdTag>, "ID Tag not found in registry.");
            }
        }
    };
} // namespace detail

/**
 * @brief An immutable, compile-time safe registry.
 * Each 'add' operation returns a new Registry object with an updated type.
 */
template<typename... Pairs>
class Registry {
private:
    std::tuple<Pairs...> m_items;

public:
    Registry(Pairs... args) : m_items(args...) {}

template<typename IdTag, typename U>
    auto add(U& object) const {
        auto new_item = std::make_pair(IdTag{}, const_cast<std::remove_const_t<U>*>(&object));
        auto new_item_tuple = std::make_tuple(new_item);
        auto combined_tuple = std::tuple_cat(m_items, new_item_tuple);

        auto factory = [](auto... args) {
            // Note the new type includes the added pair
            return Registry<Pairs..., std::pair<IdTag, std::remove_const_t<U>*>>(args...);
        };
        return std::apply(factory, combined_tuple);
    }

    template<typename IdTag>
    auto get() {
        return detail::find_by_id_tag_impl<IdTag, decltype(m_items)>::run(m_items).second;
    }
};

// =================================================================================
// PART 2: The Polymorphic State Management Layer (Type Erasure)
// =================================================================================

class IRegistryState {
public:
    virtual ~IRegistryState() = default;

    // Applies a function to the contained registry to produce the next state
    virtual std::unique_ptr<IRegistryState> apply_add(std::any adder_func) = 0;
    
    // Applies a function to get a value from the contained registry
    virtual void* apply_get(std::any getter_func) = 0;
};

template<typename... Pairs>
class RegistryState : public IRegistryState {
private:
    Registry<Pairs...> m_registry;

public:
    RegistryState(Registry<Pairs...> registry) : m_registry(std::move(registry)) {}

    std::unique_ptr<IRegistryState> apply_add(std::any adder_func) override {
        // Cast the type-erased function back to its real, specific type and call it
        using AdderFuncType = std::function<std::unique_ptr<IRegistryState>(Registry<Pairs...>&)>;
        auto func = std::any_cast<AdderFuncType>(adder_func);
        return func(m_registry);
    }

    void* apply_get(std::any getter_func) override {
        // Cast the type-erased getter function and call it
        using GetterFuncType = std::function<void*(Registry<Pairs...>&)>;
        auto func = std::any_cast<GetterFuncType>(getter_func);
        return func(m_registry);
    }
};

// =================================================================================
// PART 3: Type-Embedded Tags and Handler
// =================================================================================

// Define the Tag template that embeds type information
template<typename T>
struct Tag {
    using type = T;
};

// Create template variable for easy tag creation
template<typename T>
inline constexpr Tag<T> tag{};

/**
 * @brief A convenient handler that provides a simple interface to the registry system.
 * This version uses type-embedded tags for automatic type deduction.
 */
class RegistryHandler {
private:
    std::unordered_map<std::type_index, void*> m_storage;

public:
    // Add with type-embedded tag
    template<typename T, typename U>
    void add(Tag<T> tag_instance, U& object) {
        static_assert(std::is_same_v<T, std::remove_const_t<U>>, "Tag type must match object type (ignoring const)");
        std::type_index key{typeid(tag_instance)};
        m_storage[key] = const_cast<void*>(static_cast<const void*>(&object));
    }

    // Get with automatic type deduction from tag
    template<typename T>
    T* get(Tag<T> tag_instance) {
        std::type_index key{typeid(tag_instance)};
        auto it = m_storage.find(key);
        if (it != m_storage.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
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
// HOW TO USE IT: A Complete Example with Type-Embedded Tags
// =================================================================================

// Create specific tag instances with embedded type information
#define MAKE_TAG(NAME, TYP) constexpr auto NAME = Tag<TYP>{};

constexpr auto PlayerNameTag = Tag<std::string>{};
constexpr auto ScoreTag = Tag<int>{};
constexpr auto IsActiveTag = Tag<bool>{};
constexpr auto GravityTag = Tag<double>{};

int main() {
    // 2. Create the objects you want to manage.
    std::string player_name = "Zürich_Champion";
    int score = 2025;
    bool is_active = true;
    const double gravity = 9.81;

    // 3. Create a single handler object. Its type never changes.
    RegistryHandler handler;
    std::cout << "Handler created. Populating with data..." << std::endl;

    // 4. Add entries using type-embedded tags - no need to specify types separately!
    handler.add(PlayerNameTag, player_name);
    handler.add(ScoreTag, score);
    handler.add(IsActiveTag, is_active);
    handler.add(GravityTag, gravity);

    std::cout << "\n--- Values Retrieved with Type-Embedded Tags ---" << std::endl;

    // Method 1: Direct get() calls - type is automatically deduced from tag
    std::cout << "Method 1 - Direct get() with type-embedded tags:" << std::endl;
    std::string* name_ptr = handler.get(PlayerNameTag);
    int* score_ptr = handler.get(ScoreTag);
    bool* active_ptr = handler.get(IsActiveTag);
    const double* gravity_ptr = handler.get(GravityTag);
    
    if (name_ptr && score_ptr && active_ptr && gravity_ptr) {
        std::cout << "  Player Name: " << *name_ptr << std::endl;
        std::cout << "  Score:       " << *score_ptr << std::endl;
        std::cout << "  Is Active:   " << std::boolalpha << *active_ptr << std::endl;
        std::cout << "  Gravity:     " << *gravity_ptr << std::endl;
    }

    // Method 2: Using operator[] - even more elegant!
    std::cout << "\nMethod 2 - operator[] with type-embedded tags:" << std::endl;
    std::string* name_ptr2 = handler[PlayerNameTag];
    int* score_ptr2 = handler[ScoreTag];
    bool* active_ptr2 = handler[IsActiveTag];
    
    if (name_ptr2 && score_ptr2 && active_ptr2) {
        std::cout << "  Player Name: " << *name_ptr2 << std::endl;
        std::cout << "  Score:       " << *score_ptr2 << std::endl;
        std::cout << "  Is Active:   " << std::boolalpha << *active_ptr2 << std::endl;
    }

    // Method 3: Using the template variable approach
    std::cout << "\nMethod 3 - Using template variable tags:" << std::endl;
    handler.add(tag<std::string>, player_name);  // Alternative tag for same object
    auto alt_name_ptr = handler.get(tag<std::string>);
    if (alt_name_ptr) {
        std::cout << "  Alternative Name Access: " << *alt_name_ptr << std::endl;
    }

    // 5. Modify data through the retrieved pointers.
    std::cout << "\n--- Modifying Data Through Handler ---" << std::endl;
    std::cout << "Setting score to 9001..." << std::endl;
    *score_ptr = 9001;
    
    // 6. Verify that the original objects have been changed.
    std::cout << "\n--- Original Variables After Modification ---" << std::endl;
    std::cout << "The original 'score' variable is now: " << score << std::endl;

    // 7. Demonstrate type safety - this won't compile:
    // auto wrong_ptr = handler.get<PlayerNameTag, int>(); // ERROR: types don't match!

    std::cout << "\n--- Demonstrating Tag Uniqueness ---" << std::endl;
    std::cout << "Contains PlayerNameTag: " << std::boolalpha << handler.contains(PlayerNameTag) << std::endl;
    std::cout << "Contains ScoreTag: " << std::boolalpha << handler.contains(ScoreTag) << std::endl;

    return 0;
}