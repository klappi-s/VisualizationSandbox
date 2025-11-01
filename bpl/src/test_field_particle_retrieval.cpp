// Test program to demonstrate field and particle retrieval by ID functionality
#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>

// User-defined types (must be defined before including bpl.h)
constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main() {
    std::cout << "=== Field and Particle Retrieval by ID Demo ===" << std::endl;
    
    // Create a registry instance
    using vb_t = VisRegistry<T, Dim>;
    vb_t registry;
    
    std::cout << "\n--- Creating Test Fields ---" << std::endl;
    
    // Create some test fields
    Field<T, Dim> scalar_field1("temperature", 25.5);
    Field<T, Dim> scalar_field2("pressure", 101325.0);
    Field<T, 2> scalar_field_2d("area", 45.2);
    
    // Create vector fields
    Field<vec<T, 3>, Dim> vector_field1("velocity", {{10.0, 5.0, 2.0}});
    Field<vec<T, 2>, 2> vector_field_2d("gradient", {{1.5, 2.5}});
    
    std::cout << "\n--- Creating Test Particles ---" << std::endl;
    
    // Create some test particles  
    ParticleBase<T, Dim> particle1("electron_beam", {{1.0, 2.0, 3.0}});
    ParticleBase<T, Dim> particle2("proton_bunch", {{4.0, 5.0, 6.0}});
    








    
    std::cout << "\n=== FIELD RETRIEVAL TESTS ===" << std::endl;
    
    // Test scalar field retrieval
    std::cout << "\n--- Testing Scalar Field Retrieval ---" << std::endl;
    
    auto* temp_field = vb_t::findScalarFieldByID("temperature");
    if (temp_field) {
        std::cout << "✓ Found scalar field 'temperature': " << temp_field->data << std::endl;
    } else {
        std::cout << "✗ Could not find scalar field 'temperature'" << std::endl;
    }
    
    auto* pressure_field = vb_t::findScalarFieldByID("pressure");
    if (pressure_field) {
        std::cout << "✓ Found scalar field 'pressure': " << pressure_field->data << std::endl;
    }
    
    auto* area_field = vb_t::findScalarFieldByID<2>("area");
    if (area_field) {
        std::cout << "✓ Found 2D scalar field 'area': " << area_field->data << std::endl;
    }
    
    // Test vector field retrieval
    std::cout << "\n--- Testing Vector Field Retrieval ---" << std::endl;
    
    auto* velocity_field = vb_t::findVectorFieldByID<3, 3>("velocity");
    if (velocity_field) {
        std::cout << "✓ Found vector field 'velocity': [" 
                  << velocity_field->data[0] << ", " 
                  << velocity_field->data[1] << ", " 
                  << velocity_field->data[2] << "]" << std::endl;
    } else {
        std::cout << "✗ Could not find vector field 'velocity'" << std::endl;
    }
    
    auto* gradient_field = vb_t::findVectorFieldByID<2, 2>("gradient");
    if (gradient_field) {
        std::cout << "✓ Found 2D vector field 'gradient': [" 
                  << gradient_field->data[0] << ", " 
                  << gradient_field->data[1] << "]" << std::endl;
    }
    
    // Test generic field retrieval
    std::cout << "\n--- Testing Generic Field Retrieval ---" << std::endl;
    
    void* generic_field = vb_t::findFieldByID("temperature");
    if (generic_field) {
        std::cout << "✓ Found field using generic search: 'temperature'" << std::endl;
    }
    
    std::cout << "\n=== PARTICLE RETRIEVAL TESTS ===" << std::endl;
    
    // Test particle retrieval
    auto* electron_particle = vb_t::findParticleByID("electron_beam");
    if (electron_particle) {
        std::cout << "✓ Found particle 'electron_beam' with " 
                  << electron_particle->values.size() << " values" << std::endl;
        if (!electron_particle->values.empty()) {
            std::cout << "  First value: [" 
                      << electron_particle->values[0][0] << ", "
                      << electron_particle->values[0][1] << ", "
                      << electron_particle->values[0][2] << "]" << std::endl;
        }
    } else {
        std::cout << "✗ Could not find particle 'electron_beam'" << std::endl;
    }
    
    auto* proton_particle = vb_t::findParticleByID("proton_bunch");
    if (proton_particle) {
        std::cout << "✓ Found particle 'proton_bunch'" << std::endl;
    }
    
    std::cout << "\n=== UTILITY FUNCTION TESTS ===" << std::endl;
    
    // Test data retrieval with std::optional
    auto temp_data = vb_t::getFieldData<T>("temperature");
    if (temp_data.has_value()) {
        std::cout << "✓ Retrieved temperature data: " << temp_data.value() << std::endl;
    }
    
    auto velocity_data = vb_t::getVectorFieldData<3>("velocity");
    if (velocity_data.has_value()) {
        std::cout << "✓ Retrieved velocity data: [" 
                  << velocity_data.value()[0] << ", " 
                  << velocity_data.value()[1] << ", " 
                  << velocity_data.value()[2] << "]" << std::endl;
    }
    
    auto particle_data = vb_t::getParticleData("electron_beam");
    if (particle_data.has_value()) {
        std::cout << "✓ Retrieved particle data with " 
                  << particle_data.value().size() << " entries" << std::endl;
    }
    
    std::cout << "\n--- Information Printing ---" << std::endl;
    
    // Test information printing
    vb_t::printFieldInfo("temperature");
    std::cout << std::endl;
    vb_t::printParticleInfo("electron_beam");
    
    std::cout << "\n--- Testing Non-existent IDs ---" << std::endl;
    
    // Test with non-existent IDs
    auto* missing_field = vb_t::findFieldByID("non_existent");
    if (!missing_field) {
        std::cout << "✓ Correctly returned nullptr for non-existent field" << std::endl;
    }
    
    auto* missing_particle = vb_t::findParticleByID("non_existent");
    if (!missing_particle) {
        std::cout << "✓ Correctly returned nullptr for non-existent particle" << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}
