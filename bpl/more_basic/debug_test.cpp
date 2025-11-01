// Simple debug test
#include <iostream>
#include <cassert>

constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main() {
    std::cout << "=== Debug Test ===" << std::endl;
    
    bpl::initializeVisualization();
    std::cout << "Manager created" << std::endl;
    
    std::cout << "Container sizes before: fields=" << bpl::vb->get_size_sf() 
              << ", particles=" << bpl::vb->get_size_pb() << std::endl;
    
    try {
        Field<double, 3> f1("test_field", 25.5);
        std::cout << "Field created successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Field creation failed: " << e.what() << std::endl;
    }
    
    std::cout << "Container sizes after: fields=" << bpl::vb->get_size_sf() 
              << ", particles=" << bpl::vb->get_size_pb() << std::endl;
    
    std::cout << "Trying to access field container..." << std::endl;
    
    if (!VisBase::sf_c.empty()) {
        std::cout << "Container is not empty, size: " << VisBase::sf_c.size() << std::endl;
        if (VisBase::sf_c[0]) {
            std::cout << "First element exists, field_ID: " << VisBase::sf_c[0]->field_ID << std::endl;
        } else {
            std::cout << "First element is nullptr!" << std::endl;
        }
    } else {
        std::cout << "Container is empty!" << std::endl;
    }
    
    std::cout << "=== Debug Complete ===" << std::endl;
    return 0;
}
