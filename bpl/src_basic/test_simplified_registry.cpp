// Test program for simplified type-constrained registry
#include <iostream>
#include <cassert>

// Define the allowed types - this must come BEFORE including bpl.h
constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main() {
    std::cout << "=== Simplified Type-Constrained Registry Demo ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created" << std::endl;
    
    std::cout << "\n--- Creating Valid Types ---" << std::endl;
    
    try {
        // These should work - matching T=double, Dim=3
        Field<double, 3> f1("temperature", 25.5);
        Field<vec<double, 3>, 3> vf1("velocity", {{1.0, 2.0, 3.0}});
        ParticleBase<double, 3> p1("electrons", 1.0);
        
        std::cout << "✓ Successfully created valid types" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Unexpected error with valid types: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Testing Invalid Types (should fail) ---" << std::endl;
    
    // Test invalid scalar type
    try {
        Field<float, 3> f2("invalid_scalar", 1.0f);
        std::cout << "✗ ERROR: Should not have accepted float type!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Correctly rejected float type: " << e.what() << std::endl;
    }
    
    // Test invalid dimension
    try {
        Field<double, 2> f3("invalid_dim", 1.0);
        std::cout << "✗ ERROR: Should not have accepted Dim=2!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Correctly rejected Dim=2: " << e.what() << std::endl;
    }
    
    // Test invalid particle type
    try {
        ParticleBase<float, 3> p2("invalid_particle", 1.0f);
        std::cout << "✗ ERROR: Should not have accepted particle with float type!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Correctly rejected particle float type: " << e.what() << std::endl;
    }
    
    // Test invalid vector field scalar type
    try {
        Field<vec<float, 3>, 3> vf2("invalid_vector", {{1.0f, 2.0f, 3.0f}});
        std::cout << "✗ ERROR: Should not have accepted vector field with float components!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Correctly rejected vector field with float components: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Registry Status ---" << std::endl;
    std::cout << "Registered fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Registered particles: " << bpl::vb->get_size_pb() << std::endl;
    
    VisBase::listAllFields();
    VisBase::listAllParticles();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
