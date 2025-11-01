// Test file to demonstrate internal error handling in BPL library
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
    std::cout << "=== Internal Error Handling Demo ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created" << std::endl;
    
    std::cout << "\nInitial container sizes:" << std::endl;
    std::cout << "Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "\n--- Creating Valid Objects ---" << std::endl;
    
    // Create valid fields - these should work
    Field<T,Dim> f0;
    Field<T,Dim> f1("a Name", 1.0);
    Field<T,Dim> f2("another Name", 2.0);
    
    // Create valid particles - these should work
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
    
    std::cout << "\n--- Testing Internal Error Handling ---" << std::endl;
    
    // Try to create invalid types - these should be handled internally
    std::cout << "Trying to create invalid type Field<float, 3>..." << std::endl;
    Field<float, 3> invalid_field("should_fail", 1.0f);
    std::cout << "Object created but registration status: " << (invalid_field.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    
    std::cout << "\nTrying to create invalid dimension Field<double, 2>..." << std::endl;
    Field<double, 2> invalid_dim("should_fail", 1.0);
    std::cout << "Object created but registration status: " << (invalid_dim.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    
    std::cout << "\nTrying to create invalid particle ParticleBase<float, 3>..." << std::endl;
    ParticleBase<float, 3> invalid_particle("invalid_particle", 1.0f);
    std::cout << "Object created but registration status: " << (invalid_particle.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    
    std::cout << "\n--- Checking Registration Status of Valid Objects ---" << std::endl;
    std::cout << "f1 registration status: " << (f1.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    std::cout << "p2 registration status: " << (p2.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    std::cout << "vfield registration status: " << (vfield.isRegistered() ? "REGISTERED" : "NOT REGISTERED") << std::endl;
    
    std::cout << "\nFinal container sizes:" << std::endl;
    std::cout << "Fields: " << bpl::vb->get_size_sf() << std::endl;
    std::cout << "Particles: " << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "No exceptions were thrown to user code!" << std::endl;
    return 0;
}
