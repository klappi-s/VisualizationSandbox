#include <iostream>
constexpr unsigned Dim = 2;  // Different dimension
using T = double;
#include "bpl.h"
int main() {
    bpl::initializeVisualization();
    try {
        Field<double, 3> f("test", 1.0);  // Mismatch: expects Dim=2 but got 3
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
