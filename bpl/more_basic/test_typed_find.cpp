// Test program for typed find functions
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
    std::cout << "=== Typed Find Functions Demo ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created\n" << std::endl;
    
    std::cout << "--- Creating Test Objects ---" << std::endl;
    
    // Create valid fields
    Field<T,Dim> scalar_field("temperature", 25.5);
    Field<vec<T,3>, Dim> vector_field("velocity", {{1.0, 2.0, 3.0}});
    
    // Create valid particles
    ParticleBase<T,Dim> particle("electrons", 1.0);
    
    std::cout << "\nContainer sizes:" << std::endl;
    std::cout << "Scalar Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Vector Fields: " << bpl::vb->get_size_vf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "\n--- Testing Base Type Find Functions ---" << std::endl;
    
    // Test base type find functions
    Field_b* found_base_field = VisBase::findFieldByID("temperature");
    if (found_base_field) {
        std::cout << "✓ Found field (base type): " << found_base_field->field_ID << std::endl;
    } else {
        std::cout << "✗ Field not found" << std::endl;
    }
    
    ParticleBase_b* found_base_particle = VisBase::findParticleByID("electrons");
    if (found_base_particle) {
        std::cout << "✓ Found particle (base type): " << found_base_particle->bunch_ID << std::endl;
    } else {
        std::cout << "✗ Particle not found" << std::endl;
    }
    
    std::cout << "\n--- Testing Typed Find Functions ---" << std::endl;
    
    // Test typed find functions
    Field<T,Dim>* found_typed_scalar = VisBase::findTypedScalarFieldByID<T,Dim>("temperature");
    if (found_typed_scalar) {
        std::cout << "✓ Found scalar field (typed): " << found_typed_scalar->field_ID 
                  << ", data = " << found_typed_scalar->data << std::endl;
    } else {
        std::cout << "✗ Typed scalar field not found" << std::endl;
    }
    
    Field<vec<T,3>,Dim>* found_typed_vector = VisBase::findTypedVectorFieldByID<vec<T,3>,Dim>("velocity");
    if (found_typed_vector) {
        std::cout << "✓ Found vector field (typed): " << found_typed_vector->field_ID 
                  << ", data = [" << found_typed_vector->data[0] << ", " 
                  << found_typed_vector->data[1] << ", " 
                  << found_typed_vector->data[2] << "]" << std::endl;
    } else {
        std::cout << "✗ Typed vector field not found" << std::endl;
    }
    
    ParticleBase<T,Dim>* found_typed_particle = VisBase::findTypedParticleByID<T,Dim>("electrons");
    if (found_typed_particle) {
        std::cout << "✓ Found particle (typed): " << found_typed_particle->bunch_ID 
                  << ", data = " << found_typed_particle->data << std::endl;
    } else {
        std::cout << "✗ Typed particle not found" << std::endl;
    }
    
    std::cout << "\n--- Testing Generic Typed Find Function ---" << std::endl;
    
    // Test the generic typed find function
    Field<T,Dim>* found_generic = VisBase::findTypedFieldByID<T,Dim>("temperature");
    if (found_generic) {
        std::cout << "✓ Found field via generic typed find: " << found_generic->field_ID 
                  << ", data = " << found_generic->data << std::endl;
    } else {
        std::cout << "✗ Generic typed find failed" << std::endl;
    }
    
    std::cout << "\n--- Testing Type Safety ---" << std::endl;
    
    // Test type safety - try to find with wrong type
    Field<float,Dim>* wrong_type = VisBase::findTypedScalarFieldByID<float,Dim>("temperature");
    if (wrong_type) {
        std::cout << "✗ ERROR: Found field with wrong type!" << std::endl;
    } else {
        std::cout << "✓ Correctly rejected wrong type query" << std::endl;
    }
    
    // Test with wrong dimension
    Field<T,2>* wrong_dim = VisBase::findTypedScalarFieldByID<T,2>("temperature");
    if (wrong_dim) {
        std::cout << "✗ ERROR: Found field with wrong dimension!" << std::endl;
    } else {
        std::cout << "✓ Correctly rejected wrong dimension query" << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "✓ Typed find functions provide type safety and direct access to derived types!" << std::endl;
    return 0;
}
