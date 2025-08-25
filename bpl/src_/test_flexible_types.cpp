// Test file to demonstrate the flexibility of the new template-based approach
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

// User-defined types - different from amain.cpp
constexpr unsigned Dim = 2;  // Different dimension
using T = float;             // Different scalar type

#include "bpl.h"

int main(){
    
    bpl::initializeVisualization();
    
    std::cout << "=== Testing with float and Dim=2 ===" << std::endl;
    
    // Create fields with different user types
    Field<T,Dim> f1("float_field", 3.14f);
    Field<vec<T,3>, Dim> vec_field("vec_float_field", {{1.1f, 2.2f, 3.3f}});
    
    // Test field retrieval
    VisBase::listAllFields();
    
    // Test scalar field retrieval
    std::cout << "\n=== Float Field Data ===" << std::endl;
    std::any data = VisBase::getFieldData("float_field");
    if (data.has_value()) {
        try {
            float value = std::any_cast<float>(data);
            std::cout << "Retrieved float value: " << value << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "Failed to cast float data: " << e.what() << std::endl;
        }
    }
    
    // Test vector field retrieval
    std::cout << "\n=== Vector Float Field Data ===" << std::endl;
    std::any vec_data = VisBase::getFieldData("vec_float_field");
    if (vec_data.has_value()) {
        try {
            auto vec_value = std::any_cast<vec<float, 3>>(vec_data);
            std::cout << "Retrieved vector: (" << vec_value[0] << ", " 
                      << vec_value[1] << ", " << vec_value[2] << ")" << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "Failed to cast vector data: " << e.what() << std::endl;
        }
    }

    return 0;
}
