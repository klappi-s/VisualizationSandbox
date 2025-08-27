// Standalone test for enhanced VisBase functionality
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <optional>

constexpr unsigned Dim = 3;
using T = double;


    #include "bpl.h"

int main() {


    // std::cout << "=== BPL Enhanced Auto Interface Demo ===" << std::endl;
    
    // bpl::initializeVisualization();
    
    // // Create some test fields with different types
    // Field<double, Dim> scalar_field("temperature", 42.5);
    // Field<int, Dim> integer_field("particle_count", 1000);
    // Field<vec<double, 3>, Dim> vector_field("velocity", {{1.5, 2.0, 3.2}});
    // Field<vec<float, 2>, Dim> vector2d_field("position_2d", {{10.0f, 20.0f}});
    
    // std::cout << "Created " << bpl::vb->get_size_sf() << " fields" << std::endl;
    
    // // List all fields
    // std::cout << "\n=== All Fields ===" << std::endl;
    // VisBase::listAllFields();
    
    // // ====================================================================
    // // USER-FRIENDLY AUTO INTERFACE DEMONSTRATION
    // // ====================================================================
    
    // std::cout << "\n=== User-Friendly Auto Interface ===" << std::endl;
    


    // // 1. Simple auto declarations without manual type casting
    // std::cout << "\n1. Auto declarations:" << std::endl;

    
    // auto temperature = bpl::vb->getField<double>("temperature");
    // auto count = bpl::vb->getField<int>("particle_count");
    // auto velocity = bpl::vb->getField<vec<double, 3>>("velocity");


    // if (temperature) {
    //     std::cout << "Temperature: " << *temperature << std::endl;
    // } else {
    //     std::cout << "Failed to get temperature field" << std::endl;
    // }
    
    // if (count) {
    //     std::cout << "Particle count: " << *count << std::endl;
    // } else {
    //     std::cout << "Failed to get particle_count field" << std::endl;
    // }
    
    // if (velocity) {
    //     std::cout << "Velocity: (" << (*velocity)[0] << ", " 
    //               << (*velocity)[1] << ", " << (*velocity)[2] << ")" << std::endl;
    // } else {
    //     std::cout << "Failed to get velocity field" << std::endl;
    // }








    
    // // 2. Convenient helper methods with type inference
    // std::cout << "\n2. Type-specific helpers:" << std::endl;
    
    // auto temp2 = bpl::vb->getScalarField<double, Dim>("temperature");
    // auto vel2  = bpl::vb->getVectorField<double, 3, Dim>("velocity");

    // if (temp2) {
    //     std::cout << "Temperature (scalar): " << *temp2 << std::endl;
    // }
    
    // if (vel2) {
    //     std::cout << "Velocity (vector): (" << (*vel2)[0] << ", " 
    //               << (*vel2)[1] << ", " << (*vel2)[2] << ")" << std::endl;
    // }





    
    // // 3. Safe access with default values
    // std::cout << "\n3. Safe access with defaults:" << std::endl;
    
    // double safe_temp = bpl::vb->getFieldOr("temperature", 0.0);
    // std::cout << "Temperature (safe): " << safe_temp << std::endl;
    
    // double missing = bpl::vb->getFieldOr("nonexistent", 99.9);
    // std::cout << "Missing field with default: " << missing << std::endl;
    
    // // 4. Field existence checks
    // std::cout << "\n4. Field existence checks:" << std::endl;
    
    // if (bpl::vb->hasField("temperature")) {
    //     std::cout << "✓ Temperature field exists" << std::endl;
    // }
    
    // if (!bpl::vb->hasField("pressure")) {
    //     std::cout << "✗ Pressure field does not exist" << std::endl;
    // }
    
    // // 5. List fields by type
    // std::cout << "\n5. List fields by type:" << std::endl;
    
    // auto double_fields = bpl::vb->listFieldsOfType<double, Dim>();
    // std::cout << "Double fields: ";
    // for (const auto& field_name : double_fields) {
    //     std::cout << field_name << " ";
    // }
    // std::cout << std::endl;
    
    // auto vector_fields = bpl::vb->listFieldsOfType<vec<double, 3>, Dim>();
    // std::cout << "Vector<double,3> fields: ";
    // for (const auto& field_name : vector_fields) {
    //     std::cout << field_name << " ";
    // }
    // std::cout << std::endl;
    
    // // ====================================================================
    // // ERROR HANDLING DEMONSTRATION
    // // ====================================================================
    
    // std::cout << "\n=== Error Handling Demo ===" << std::endl;
    
    // // Try to get a field with wrong type (should fail gracefully)
    // auto wrong_type = bpl::vb->getField<int>("temperature");  // temperature is double, not int
    // if (!wrong_type) {
    //     std::cout << "✓ Correctly handled type mismatch for temperature as int" << std::endl;
    // }
    
    // // Try to get non-existent field
    // auto missing_field = bpl::vb->getField<double>("nonexistent");
    // if (!missing_field) {
    //     std::cout << "✓ Correctly handled missing field" << std::endl;
    // }
    
    // // ====================================================================
    // // ADVANCED USER SCENARIOS
    // // ====================================================================
    
    // std::cout << "\n=== Advanced Scenarios ===" << std::endl;
    
    // // User working with multiple field types in a loop
    // std::vector<std::string> field_names = {"temperature", "particle_count", "velocity"};
    
    // for (const auto& name : field_names) {
    //     std::cout << "Field '" << name << "': ";
        
    //     // Try different types and print what works
    //     if (auto val = bpl::vb->getField<double>(name)) {
    //         std::cout << "double = " << *val << std::endl;
    //     } else if (auto val = bpl::vb->getField<int>(name)) {
    //         std::cout << "int = " << *val << std::endl;
    //     } else if (auto val = bpl::vb->getField<vec<double, 3>>(name)) {
    //         std::cout << "vec3d = (" << (*val)[0] << ", " << (*val)[1] << ", " << (*val)[2] << ")" << std::endl;
    //     } else {
    //         std::cout << "unknown or unsupported type" << std::endl;
    //     }
    // }
    
    // // ====================================================================
    // // COMPARISON WITH OLD API
    // // ====================================================================
    
    // std::cout << "\n=== Comparison: Old vs New API ===" << std::endl;
    
    // std::cout << "\nOLD API (manual casting required):" << std::endl;
    // std::any old_data = VisBase::getFieldData("temperature");
    // if (old_data.has_value()) {
    //     try {
    //         double old_value = std::any_cast<double>(old_data);
    //         std::cout << "Old API temperature: " << old_value << std::endl;
    //     } catch (const std::bad_any_cast& e) {
    //         std::cout << "Old API failed: " << e.what() << std::endl;
    //     }
    // }
    
    // std::cout << "\nNEW API (auto-compatible):" << std::endl;
    // auto new_temp = bpl::vb->getField<double>("temperature");  // auto deduces std::optional<double>
    // if (new_temp) {
    //     std::cout << "New API temperature: " << *new_temp << std::endl;
    // }
    
    // std::cout << "\n=== Demo Complete ===" << std::endl;
    // std::cout << "The enhanced API provides:" << std::endl;
    // std::cout << "✓ Type-safe field access" << std::endl;
    // std::cout << "✓ Auto-compatible return types" << std::endl;
    // std::cout << "✓ Internal error handling" << std::endl;
    // std::cout << "✓ No manual type casting required" << std::endl;
    // std::cout << "✓ Optional-based safe access patterns" << std::endl;
    
    return 0;
}
