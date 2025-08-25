// Simplified Type-Constrained Registry - Main Demo
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

// Define the allowed types for the entire application
// These MUST be defined before including bpl.h
constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){
    std::cout << "=== Simplified BPL Registry Demo ===" << std::endl;
    std::cout << "Configured for: T=" << typeid(T).name() << ", Dim=" << Dim << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Registry manager created\n" << std::endl;
    
    std::cout << "Initial container sizes:" << std::endl;
    std::cout << "  Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "  Particles: " << bpl::vb->get_size_pb() << std::endl;

    std::cout << "\n--- Creating Valid Objects ---" << std::endl;
    
    // Valid scalar fields
    Field<T,Dim> f1("temperature", 25.5);
    Field<T,Dim> f2("pressure", 101325.0);
    
    // Valid vector field
    Field<vec<T,3>, Dim> vf1("velocity", {{10.0, 5.0, 2.0}});
    
    // Valid particles
    ParticleBase<T,Dim> p1("electrons", 1.0);
    ParticleBase<T,Dim> p2("protons", 2.0);
    
    std::cout << "\n✓ All valid objects created successfully!" << std::endl;
    
    std::cout << "\nUpdated container sizes:" << std::endl;
    std::cout << "  Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "  Particles: " << bpl::vb->get_size_pb() << std::endl;

    // Display all registered objects
    VisBase::listAllFields();
    VisBase::listAllParticles();

    std::cout << "\n--- Testing Field Retrieval ---" << std::endl;
    Field_b* found_field = VisBase::findFieldByID("temperature");
    if (found_field) {
        std::cout << "✓ Found field: " << found_field->field_ID << std::endl;
    } else {
        std::cout << "✗ Failed to find field" << std::endl;
    }

    std::cout << "\n--- Testing Particle Retrieval ---" << std::endl;
    ParticleBase_b* found_particle = VisBase::findParticleByID("electrons");
    if (found_particle) {
        std::cout << "✓ Found particle: " << found_particle->bunch_ID << std::endl;
    } else {
        std::cout << "✗ Failed to find particle" << std::endl;
    }

    std::cout << "\n--- Attempting Invalid Registrations (handled internally) ---" << std::endl;
    
    // These invalid objects will be created but not registered
    // No try-catch blocks needed anymore!
    std::cout << "Creating Field<float, 3>..." << std::endl;
    Field<float, 3> invalid_field("should_fail", 1.0f);
    std::cout << "Result: Object created, registered = " << (invalid_field.isRegistered() ? "YES" : "NO") << std::endl;
    
    std::cout << "\nCreating Field<double, 2>..." << std::endl;
    Field<double, 2> invalid_dim("should_fail", 1.0);
    std::cout << "Result: Object created, registered = " << (invalid_dim.isRegistered() ? "YES" : "NO") << std::endl;
    
    std::cout << "\nCreating ParticleBase<float, 3>..." << std::endl;
    ParticleBase<float, 3> invalid_particle("should_fail", 1.0f);
    std::cout << "Result: Object created, registered = " << (invalid_particle.isRegistered() ? "YES" : "NO") << std::endl;

    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "✓ All validation errors handled internally by the library!" << std::endl;
    std::cout << "Registry successfully enforced type constraints based on user-defined T=" 
              << typeid(T).name() << " and Dim=" << Dim << std::endl;

    return 0;
}
