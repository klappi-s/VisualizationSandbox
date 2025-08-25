#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <cxxabi.h>

// Forward declarations needed for VisBase.h
template<typename T, unsigned Dim>
class Field;

template<typename T, unsigned Dim>  
class ParticleBase;

template<typename T, unsigned Dim>
struct vec;

#include "VisBase.h"
#include "VisBase.hpp"

int main() {
    std::cout << "Testing VisRegistry print_state() functionality (Simplified)\n";
    std::cout << "===========================================================\n\n";
    
    // Create a VisRegistry instance
    VisRegistry<double, 3> registry;
    
    std::cout << "1. Testing empty registry:\n";
    registry.print_state();
    
    // std::cout << "\n2. Testing different registry types:\n\n";
    
    // // Test with float and different dimensions
    // VisRegistry<float, 2> float_registry;
    // std::cout << "Float registry (Dim=2):\n";
    // float_registry.print_state();
    
    // VisRegistry<int, 1> int_registry;
    // std::cout << "Integer registry (Dim=1):\n";
    // int_registry.print_state();
    
    std::cout << "=================================================\n";
    std::cout << "print_state() implementation test completed!\n";
    std::cout << "=================================================\n";
    
    return 0;
}
