#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <initializer_list>

// A sample struct to demonstrate storing custom types.
struct Player {
    std::string name;
    int score;
};

class ImmutableRegistry {
public:
    // Helper struct to enable type-erasure within an initializer list.
    // The compiler can implicitly convert {"name", &ptr} into this struct.
    struct Entry {
        std::string name;
        std::any ptr_any;

        template <typename T>
        Entry(const std::string& entry_name, T* ptr)
            : name(entry_name), ptr_any(ptr) {}
    };

    /**
     * @brief Constructs the registry from an initializer list of entries.
     * @param entries A list like { {"id1", &obj1}, {"id2", &obj2} }.
     * @throws std::invalid_argument if duplicate names are found in the list.
     */
    ImmutableRegistry(std::initializer_list<Entry> entries)
        : m_registry(Build(entries)) {}

    /**
     * @brief Retrieves a pointer to an object by its name.
     * @tparam T The expected type of the object.
     * @param name The name of the object to retrieve.
     * @return A pointer of type T* if found and the type matches, otherwise nullptr.
     */
    template <typename T>
    T* Get(const std::string& name) const {
        auto it = m_registry.find(name);
        if (it == m_registry.end()) {
            return nullptr; // Object not found.
        }

        try {
            // std::any_cast will throw if the stored type is not T*
            return std::any_cast<T*>(it->second);
        } catch (const std::bad_any_cast&) {
            // This indicates a type mismatch.
            return nullptr;
        }
    }

    /**
     * @brief Checks if an object with the given name is registered.
     * @param name The name to check.
     * @return True if the name exists in the registry, false otherwise.
     */
    bool Exists(const std::string& name) const {
        return m_registry.count(name);
    }

private:
    // Helper to build the underlying map while checking duplicates.
    static std::unordered_map<std::string, std::any>
    Build(std::initializer_list<Entry> entries) {
        std::unordered_map<std::string, std::any> temp_registry;
        for (const auto& entry : entries) {
            if (temp_registry.count(entry.name)) {
                throw std::invalid_argument(
                    std::string("Duplicate name provided to ImmutableRegistry: ") + entry.name);
            }
            temp_registry[entry.name] = entry.ptr_any;
        }
        return temp_registry;
    }

    // The map is const, enforcing immutability after construction.
    const std::unordered_map<std::string, std::any> m_registry;
};


// --- DEMONSTRATION ---
int main() {
    // Create some objects of different types.
    Player p1 = {"Alice", 100};
    int game_level = 5;
    std::string title = "The Final Quest";

    std::cout << "## Constructing the immutable registry..." << std::endl;
    // --- Construct the registry with all objects at once ---
    const ImmutableRegistry registry = {
        {"main_player", &p1},
        {"current_level", &game_level},
        {"game_title", &title}
    };
    std::cout << "Registry constructed successfully.\n" << std::endl;

    // --- Retrieve and use objects ---
    std::cout << "## Retrieving objects..." << std::endl;
    if (Player* player = registry.Get<Player>("main_player")) {
        std::cout << "Player found: " << player->name << " (Score: " << player->score << ")" << std::endl;
    }

    if (int* level = registry.Get<int>("current_level")) {
        std::cout << "Current level found: " << *level << std::endl;
    }
    std::cout << std::endl;
    
    // --- Demonstrate error handling ---
    std::cout << "## Demonstrating error handling..." << std::endl;
    // 1. Try to get an object with the wrong type
    if (registry.Get<float>("current_level") == nullptr) {
        std::cout << "Correctly failed to get 'current_level' as a float*." << std::endl;
    }

    // 2. Try to construct a registry with duplicate keys
    std::cout << "\nAttempting to construct registry with duplicate keys..." << std::endl;
    try {
        ImmutableRegistry faulty_registry = {
            {"key1", &game_level},
            {"key2", &title},
            {"key1", &p1} // Duplicate name
        };
    } catch (const std::invalid_argument& e) {
        std::cout << "Successfully caught expected exception: " << e.what() << std::endl;
    }

    return 0;
}