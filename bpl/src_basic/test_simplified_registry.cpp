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
    
    // These should work - matching T=double, Dim=3
    Field<double, 3> f1("temperature", 25.5);
    Field<vec<double, 3>, 3> vf1("velocity", {{1.0, 2.0, 3.0}});
    ParticleBase<double, 3> p1("electrons", 1.0);
    
    // Check registration status
    if (f1.isRegistered() && vf1.isRegistered() && p1.isRegistered()) {
        std::cout << "✓ Successfully created and registered valid types" << std::endl;
    } else {
        std::cout << "✗ Some valid objects failed to register" << std::endl;
    }
    
    std::cout << "\n--- Testing Invalid Types (handled internally) ---" << std::endl;
    
    // Test invalid scalar type - library handles error internally
    std::cout << "Creating Field<float, 3>..." << std::endl;
    Field<float, 3> f2("invalid_scalar", 1.0f);
    std::cout << "Result: Object created, registered = " << (f2.isRegistered() ? "YES" : "NO") << std::endl;
    
    // Test invalid dimension - library handles error internally
    std::cout << "\nCreating Field<double, 2>..." << std::endl;
    Field<double, 2> f3("invalid_dim", 1.0);
    std::cout << "Result: Object created, registered = " << (f3.isRegistered() ? "YES" : "NO") << std::endl;
    
    // Test invalid particle type - library handles error internally
    std::cout << "\nCreating ParticleBase<float, 3>..." << std::endl;
    ParticleBase<float, 3> p2("invalid_particle", 1.0f);
    std::cout << "Result: Object created, registered = " << (p2.isRegistered() ? "YES" : "NO") << std::endl;
    
    // Test invalid vector field scalar type - library handles error internally
    std::cout << "\nCreating Field<vec<float, 3>, 3>..." << std::endl;
    Field<vec<float, 3>, 3> vf2("invalid_vector", {{1.0f, 2.0f, 3.0f}});
    std::cout << "Result: Object created, registered = " << (vf2.isRegistered() ? "YES" : "NO") << std::endl;
    
    std::cout << "\n--- Registry Status ---" << std::endl;
    std::cout << "Registered fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Registered particles: " << bpl::vb->get_size_pb() << std::endl;
    
    VisBase::listAllFields();
    VisBase::listAllParticles();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "✓ All validation errors handled internally by the library!" << std::endl;
    return 0;
}
