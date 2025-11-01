constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"
// Your template function
template<typename entry_t>
void func_entry(entry_t entry) {
        using EntryType = std::decay_t<decltype(entry)>;
        std::cout << "   Processing entry of type: " << typeid(EntryType).name() << "\n";
        
        // Type-specific processing
        if constexpr (std::is_same_v<EntryType, T>) {
            std::cout << "     -> Scalar value: " << entry << "\n";
        } else if constexpr (std::is_same_v<EntryType, vec<T, Dim>>) {
            std::cout << "     -> Vector value: " << entry << "\n";
        } else if constexpr (std::is_same_v<EntryType, std::string>) {
            std::cout << "     -> String value: \"" << entry << "\"\n";
        } else if constexpr (std::is_same_v<EntryType, int>) {
            std::cout << "     -> Integer value: " << entry << "\n";
        }
}



int main(){
    std::cout << "=== RegistryFluent Iteration Demo ===\n\n";

    // Create some data objects to register
    T density = 1.25;
    vec<T, Dim> velocity = {1.0, 2.0, 3.0};
    T pressure = 101.325;
    std::string material = "water";
    int particle_count = 1000;

    // Create a registry with different types
    using MyRegistry = RegistryFluent<
        Slot<"density", T>,
        Slot<"velocity", vec<T, Dim>>,
        Slot<"pressure", T>,
        Slot<"material", std::string>,
        Slot<"count", int>
    >;

    MyRegistry registry;

    // Set some entries (but not all, to show iteration with null entries)
    registry.Set<"density">(density);
    registry.Set<"velocity">(velocity);
    registry.Set<"material">(material);
    // Note: pressure and count are left unset (null)

    std::cout << "1. Basic Registry Information:\n";
    std::cout << "   Registry size: " << registry.size() << " active entries\n";
    std::cout << "   Is empty: " << (registry.empty() ? "yes" : "no") << "\n\n";

    std::cout << "2. All declared IDs:\n";
    auto all_ids = registry.getAllIds();
    std::cout << "   ";
    for (const auto& id : all_ids) {
        std::cout << "\"" << id << "\" ";
    }
    std::cout << "\n\n";

    std::cout << "3. Active (non-null) IDs:\n";
    auto active_ids = registry.getActiveIds();
    std::cout << "   ";
    for (const auto& id : active_ids) {
        std::cout << "\"" << id << "\" ";
    }
    std::cout << "\n\n";


    registry.forEach([](std::string_view id, const auto& value) {
        std::cout << "   Entry ID: " << id << "\n";
        func_entry(value);
    });

    std::cout << "\n";

    std::cout << "6. Adding more entries and re-iterating:\n";
    registry.Set<"pressure">(pressure);
    registry.Set<"count">(particle_count);
    
    std::cout << "   Registry size after adding more: " << registry.size() << " entries\n";
    std::cout << "   Updated forEach iteration:\n";
    registry.forEach([](std::string_view id, const auto& value) {
        std::cout << "     " << id << " = " << value << "\n";
    });
    std::cout << "\n";

    std::cout << "7. Using iteration to find specific entries:\n";
    bool found_velocity = false;
    T max_scalar_value = std::numeric_limits<T>::lowest();
    
    registry.forEach([&](std::string_view id, const auto& value) {
        if (id == "velocity") {
            found_velocity = true;
        }
        
        // Find maximum scalar value (works for T types)
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, T>) {
            if (value > max_scalar_value) {
                max_scalar_value = value;
            }
        }
    });
    
    std::cout << "   Found velocity entry: " << (found_velocity ? "yes" : "no") << "\n";
    std::cout << "   Maximum scalar value: " << max_scalar_value << "\n\n";

    std::cout << "8. Empty Registry Demo:\n";
    RegistryFluent<> empty_registry;
    std::cout << "   Empty registry size: " << empty_registry.size() << "\n";
    std::cout << "   Empty registry is empty: " << (empty_registry.empty() ? "yes" : "no") << "\n";
    std::cout << "   Empty registry forEach: ";
    empty_registry.forEach([](std::string_view id, const auto& value) {
        std::cout << "This should not print\n";
    });
    std::cout << "(no output, as expected)\n\n";

    std::cout << "9. Factory function with iteration:\n";
    T rho = 2.5;
    T phi = 3.14159;
    auto factory_registry = MakeRegistry<"rho", "phi">(rho, phi);
    
    std::cout << "   Factory-created registry contents:\n";
    factory_registry->forEach([](std::string_view id, const auto& value) {
        std::cout << "     " << id << " = " << value << "\n";
    });

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
