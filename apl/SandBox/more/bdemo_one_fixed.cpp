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
// PART 1: Simplified Registry with Type Erasure
// =================================================================================

/**
 * @brief A simplified registry that stores pointers with type erasure
 */
class RegistryHandler {
private:
    std::unordered_map<std::type_index, void*> m_storage;

public:
    template<typename IdTag, typename U>
    void add(U& object) {
        std::type_index key{typeid(IdTag)};
        m_storage[key] = static_cast<void*>(&object);
    }

    template<typename IdTag, typename T>
    T* get() {
        std::type_index key{typeid(IdTag)};
        auto it = m_storage.find(key);
        if (it != m_storage.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }
};

// =================================================================================
// HOW TO USE IT: A Complete Example
// =================================================================================

// 1. Define unique ID tags. These are empty structs with no runtime cost.
struct PlayerNameId {};
struct ScoreId {};
struct IsActiveId {};
struct GravityId {};

int main() {
    // 2. Create the objects you want to manage.
    std::string player_name = "Zürich_Champion";
    int score = 2025;
    bool is_active = true;
    const double gravity = 9.81;

    // 3. Create a single handler object. Its type never changes.
    RegistryHandler handler;
    std::cout << "Handler created. Populating with data..." << std::endl;

    // 4. Add entries using the convenient mutable-style API.
    //    You don't need to reassign the handler object.
    handler.add<PlayerNameId>(player_name);
    handler.add<ScoreId>(score);
    handler.add<IsActiveId>(is_active);
    handler.add<GravityId>(gravity);

    std::cout << "\n--- Initial Values Retrieved from Handler ---" << std::endl;

    // 5. Retrieve pointers using the ID tags. The 'get' call is typesafe.
    std::string* name_ptr = handler.get<PlayerNameId, std::string>();
    int* score_ptr = handler.get<ScoreId, int>();
    bool* active_ptr = handler.get<IsActiveId, bool>();

    if (name_ptr && score_ptr && active_ptr) {
        std::cout << "Player Name: " << *name_ptr << std::endl;
        std::cout << "Score:       " << *score_ptr << std::endl;
        std::cout << "Is Active:   " << std::boolalpha << *active_ptr << std::endl;

        // 6. Modify data through the retrieved pointers.
        std::cout << "\n--- Modifying Data Through Handler ---" << std::endl;
        std::cout << "Setting score to 9001..." << std::endl;
        *score_ptr = 9001;
        
        // 7. Verify that the original objects have been changed.
        std::cout << "\n--- Original Variables After Modification ---" << std::endl;
        std::cout << "The original 'score' variable is now: " << score << std::endl;
    } else {
        std::cout << "Error: Failed to retrieve some pointers!" << std::endl;
    }

    return 0;
}
