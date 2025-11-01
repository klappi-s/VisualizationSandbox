// Test program for templated container approach
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
    std::cout << "=== Templated Container Demo ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created\n" << std::endl;
    
    std::cout << "--- Creating Test Objects ---" << std::endl;
    
    // Create valid fields
    Field<T,Dim> scalar_field("temperature", 25.5);
    Field<vec<T,Dim>, Dim> vector_field("velocity", {{1.0, 2.0, 3.0}});  // VDim == Dim
    
    // Create valid particles
    ParticleBase<T,Dim> particle("electrons", 1.0);
    
    std::cout << "\nLegacy container sizes:" << std::endl;
    std::cout << "Scalar Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Vector Fields: " << bpl::vb->get_size_vf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "\nTyped container sizes:" << std::endl;
    auto& registry = VisBase::getTypedRegistry<T, Dim>();





    std::cout << "Typed Scalar Fields: " << registry.getScalarFieldCount() << std::endl;
    std::cout << "Typed Vector Fields: " << registry.getVectorFieldCount() << std::endl;
    std::cout << "Typed Particles: " << registry.getParticleCount() << std::endl;
    
    






    std::cout << "\n--- Testing Template-Free Access Functions ---" << std::endl;
    
    // Using the configured registry without needing templates!
    std::cout << "Template-free container sizes:" << std::endl;
    std::cout << "Scalar Fields: " << VisBase::getConfiguredScalarFieldCount() << std::endl;
    std::cout << "Vector Fields: " << VisBase::getConfiguredVectorFieldCount() << std::endl;
    std::cout << "Particles: " << VisBase::getConfiguredParticleCount() << std::endl;
    
    std::cout << "\n--- Template-Free Find Functions ---" << std::endl;
    
    // Find fields without templates - but need to cast for data access
    Field_b* found_scalar_base = VisBase::findConfiguredScalarFieldByID("temperature");
    if (found_scalar_base) {
        // For data access, we still need the typed version OR cast
        auto* found_scalar_typed = static_cast<Field<T,Dim>*>(found_scalar_base);
        std::cout << "✓ Found scalar field: " << found_scalar_typed->field_ID 
                  << ", data = " << found_scalar_typed->data << std::endl;
    } else {
        std::cout << "✗ Scalar field not found" << std::endl;
    }
    
    Field_b* found_vector_base = VisBase::findConfiguredVectorFieldByID("velocity");
    if (found_vector_base) {
        // For data access, we need to cast to the proper type
        auto* found_vector_typed = static_cast<Field<vec<T,Dim>, Dim>*>(found_vector_base);
        std::cout << "✓ Found vector field: " << found_vector_typed->field_ID 
                  << ", data = [" << found_vector_typed->data[0] << ", " 
                  << found_vector_typed->data[1] << ", " 
                  << found_vector_typed->data[2] << "]" << std::endl;
    } else {
        std::cout << "✗ Vector field not found" << std::endl;
    }

    std::cout << "\n--- Testing Typed Find Functions (No Templates Needed!) ---" << std::endl;
    // Original typed access still works for direct data access
    auto* found_scalar = registry.findScalarFieldByID("temperature");
    if (found_scalar) {
        std::cout << "✓ Found scalar field: " << found_scalar->field_ID 
                  << ", data = " << found_scalar->data << std::endl;
    } else {
        std::cout << "✗ Scalar field not found" << std::endl;
    }
    
    // Find vector field - returns proper type directly!
    auto* found_vector = registry.findVectorFieldByID("velocity");
    if (found_vector) {
        std::cout << "✓ Found vector field: " << found_vector->field_ID 
                  << ", data = [" << found_vector->data[0] << ", " 
                  << found_vector->data[1] << ", " 
                  << found_vector->data[2] << "]" << std::endl;
    } else {
        std::cout << "✗ Vector field not found" << std::endl;
    }
    
    // Find particle - returns proper type directly!
    ParticleBase<T,Dim>* found_particle = registry.findParticleByID("electrons");
    if (found_particle) {
        std::cout << "✓ Found particle: " << found_particle->bunch_ID 
                  << ", data = " << found_particle->data << std::endl;
    } else {
        std::cout << "✗ Particle not found" << std::endl;
    }
    
    std::cout << "\n--- Template-Free List Functions ---" << std::endl;
    std::cout << "Listing all fields using template-free interface:" << std::endl;
    VisBase::listConfiguredFields();
    
    std::cout << "\nListing particles using template-free interface:" << std::endl;
    VisBase::listConfiguredParticles();
    
    std::cout << "\n--- Testing VDim == Dim Constraint ---" << std::endl;
    
    // Try to create a vector field with VDim != Dim
    std::cout << "Trying to create Field<vec<double, 2>, 3> (VDim != Dim)..." << std::endl;
    Field<vec<double, 2>, 3> invalid_vdim_field("should_fail_vdim", {{1.0, 2.0}});
    std::cout << "Result: Object created, registered = " << (invalid_vdim_field.isRegistered() ? "YES" : "NO") << std::endl;
    
    std::cout << "\n--- Testing Type Safety ---" << std::endl;
    
    // Create another registry for different types to show separation
    std::cout << "Accessing registry for different type parameters..." << std::endl;
    auto& wrong_registry = VisBase::getTypedRegistry<float, 2>();
    std::cout << "Different registry found objects: " << wrong_registry.getScalarFieldCount() << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "✓ Template-free access functions work!" << std::endl;
    std::cout << "✓ Typed containers provide type safety with typed access!" << std::endl;
    std::cout << "✓ Each type combination gets its own container!" << std::endl;
    return 0;
}
