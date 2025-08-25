


    // Simplified amain.cpp for the type-constrained registry
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){
    std::cout << "=== Simplified Registry Demo ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created" << std::endl;
    
    std::cout << "\nInitial container sizes:" << std::endl;
    std::cout << "Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "\n--- Creating Valid Objects ---" << std::endl;
    
    // Create valid fields
    Field<T,Dim> f0;
    Field<T,Dim> f1("a Name", 1.0);
    Field<T,Dim> f2("another Name", 2.0);
    
    // Create valid particles
    ParticleBase<T,Dim> p0;
    ParticleBase<T,Dim> p1;
    ParticleBase<T,Dim> p2("P-Name", 2);
    
    // Create a valid vector field
    Field<vec<double,3>, Dim> vfield("vector_field", {{1.0, 2.0, 3.0}});
    Field<vec<double,3>, Dim> vfield2;
    
    std::cout << "\nUpdated container sizes:" << std::endl;
    std::cout << "Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    // Display all registered objects
    VisBase::listAllFields();
    VisBase::listAllParticles();
    
    std::cout << "\n--- Testing Field Retrieval ---" << std::endl;
    Field_b* found_field = VisBase::findFieldByID("a Name");
    if (found_field) {
        std::cout << "✓ Found field: " << found_field->field_ID << std::endl;
    } else {
        std::cout << "✗ Field not found" << std::endl;
    }
    
    std::cout << "\n--- Testing Particle Retrieval ---" << std::endl;
    ParticleBase_b* found_particle = VisBase::findParticleByID("P-Name");
    if (found_particle) {
        std::cout << "✓ Found particle: " << found_particle->bunch_ID << std::endl;
    } else {
        std::cout << "✗ Particle not found" << std::endl;
    }
    
    std::cout << "\n--- Testing Type Validation ---" << std::endl;
    
    try {
        std::cout << "Trying to create invalid type Field<float, 3>..." << std::endl;
        Field<float, 3> invalid_field("should_fail", 1.0f);
        std::cout << "✗ ERROR: Should have been rejected!" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "✓ Correctly rejected: " << e.what() << std::endl;
    }
    
    try {
        std::cout << "Trying to create invalid dimension Field<double, 2>..." << std::endl;
        Field<double, 2> invalid_dim("should_fail", 1.0);
        std::cout << "✗ ERROR: Should have been rejected!" << std::endl;    } catch (const std::runtime_error& e) {
        std::cout << "✓ Correctly rejected: " << e.what() << std::endl;
    }    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}

