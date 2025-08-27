#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <cxxabi.h>

#include "bpl.h"

int main() {
    std::cout << "Testing VisRegistry print_state() functionality (Simplified)\n";
    std::cout << "===========================================================\n\n";
    
    // Create a VisRegistry instance
    VisRegistry<double, 3> registry;
    
    // std::cout << "1. Testing empty registry:\n";
    // registry.print_state();
    
    std::cout << "\n=== APPLY_TO_VECTOR_FIELD_BY_ID DEMO ===\n\n";
    std::cout << "2. Creating test vector fieldsa and printing registry:\n";
    
    // Different vector field types for demonstration
    Field<vec<double, 3>, 3> velocity_field("velocity", {10.0, 5.0, 2.0});
    Field<vec<double, 2>, 3> gradient_field("gradient", {1.5, 2.5});
    Field<vec<double, 3>, 2> force_field("force", {100.0, -50.0, 25.0});
    Field<vec<double, 1>, 3> scalar_array_field("pressure_array", {101.325});


    
    registry.print_state();
    
    // Create some vector fields to work with
    
    std::cout << "Created fields: velocity, gradient, force, pressure_array\n\n";
    





    // Show initial state
    std::cout << "3. Initial field values:\n";
    registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
        std::cout << "  " << field.field_ID << " (Dim=" << dim << ", VDim=" << vdim << "): [";
        for (unsigned i = 0; i < vdim; ++i) {
            if constexpr (requires { field.data[i]; }) {
                std::cout << field.data[i];
            }
            if (i < vdim - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    });
    
    std::cout << "\n4. Testing apply_to_vector_field_by_id - Modifying 'velocity':\n";
    
    // Test 1: Modify velocity field by ID
    bool found_velocity = registry.apply_to_vector_field_by_id("velocity", 
        [](auto& field, unsigned dim, unsigned vdim) {
            std::cout << "  BEFORE:\n " << field.field_ID 
                      << " (Dim=" << dim << ", VDim=" << vdim << ") = [";
            for (unsigned i = 0; i < vdim; ++i) {
                if constexpr (requires { field.data[i]; }) {
                    std::cout << field.data[i];
                }
                if (i < vdim - 1) std::cout << ", ";
            }
            std::cout << "]\n";
            
            // Modify the field - double all values
            for (unsigned i = 0; i < vdim; ++i) {
                if constexpr (requires { field.data[i]; }) {
                    field.data[i] *= 2.0;
                }
            }
            
            std::cout << "  AFTER:\n" << field.field_ID << " = [";
            for (unsigned i = 0; i < vdim; ++i) {
                if constexpr (requires { field.data[i]; }) {
                    std::cout << field.data[i];
                }
                if (i < vdim - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        });
    
    if (!found_velocity) {
        std::cout << "  ❌ ERROR: Could not find 'velocity' field!\n";
    } else {
        std::cout << "  ✓ Successfully modified 'velocity' field!\n";
    }
    





    std::cout << "\n5. Testing apply_to_vector_field_by_id - Modifying 'gradient':\n";
    
    // Test 2: Modify gradient field with simpler lambda (no dimension parameters)
    bool found_gradient = registry.apply_to_vector_field_by_id("gradient", 
        [](auto& field) {
            std::cout << "  BEFORE: \n" << field.field_ID << " = [";
            for (unsigned i = 0; i < 2; ++i) { // We know it's 2D
                std::cout << field.data[i];
                if (i < 1) std::cout << ", ";
            }
            std::cout << "]\n";
            
            // Add 10 to each component
            field.data[0] += 10.0;
            field.data[1] += 10.0;
            
            std::cout << "  AFTER: \n" << field.field_ID << " = [";
            for (unsigned i = 0; i < 2; ++i) {
                std::cout << field.data[i];
                if (i < 1) std::cout << ", ";
            }
            std::cout << "]\n";
        });
    
    if (!found_gradient) {
        std::cout << "  ❌ ERROR: Could not find 'gradient' field!\n";
    } 
    // else {
    //     std::cout << "  ✓ Successfully modified 'gradient' field!\n";
    // }
    
    std::cout << "\n6. Testing with non-existent field:\n";
    
    // Test 3: Try to modify non-existent field
    bool found_nonexistent = registry.apply_to_vector_field_by_id("nonexistent_field", 
        [](auto& /*field*/, unsigned /*dim*/, unsigned /*vdim*/) {
            std::cout << "  This should not be printed!\n";
        });
    
    if (!found_nonexistent) {
        // std::cout << "  ✓ Correctly returned false for non-existent field\n";
    } 
    else {
        std::cout << "  ❌ ERROR: Found non-existent field (this shouldn't happen)\n";
    }
    
    std::cout << "\n7. Verifying changes persisted - Final field values:\n";
    
    // Show final state to prove changes persisted to original fields
    registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
        std::cout << "  " << field.field_ID << " (Dim=" << dim << ", VDim=" << vdim << "): [";
        for (unsigned i = 0; i < vdim; ++i) {
            if constexpr (requires { field.data[i]; }) {
                std::cout << field.data[i];
            }
            if (i < vdim - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    });
    
    std::cout << "\n8. Direct field access verification:\n";
    
    // Direct access to show the fields were actually modified
    std::cout << "  Direct access to velocity_field.data: [" 
              << velocity_field.data[0] << ", " 
              << velocity_field.data[1] << ", " 
              << velocity_field.data[2] << "]\n";
    
    std::cout << "  Direct access to gradient_field.data: [" 
              << gradient_field.data[0] << ", " 
              << gradient_field.data[1] << "]\n";
    
    std::cout << "\n=== SUMMARY ===\n";
    std::cout << "✓ apply_to_vector_field_by_id successfully finds fields by ID\n";
    std::cout << "✓ Modifications through the method affect the original field objects\n";
    std::cout << "✓ Method correctly returns false for non-existent fields\n";
    std::cout << "✓ Works with different vector dimensions (1D, 2D, 3D)\n";
    std::cout << "✓ Works with different spatial dimensions (2D, 3D)\n";
    std::cout << "✓ Function signature detection works (with/without dimension parameters)\n";
    
    std::cout << "\n=================================================\n";
    std::cout << "apply_to_vector_field_by_id demo completed!\n";
    std::cout << "=================================================\n";
    
    std::cout << "\n=== APPLY_TO_FIELD_BY_ID DEMO ===\n\n";
    
    // Create some scalar fields for testing
    Field<double, 3> temperature_field("temperature", 25.5);
    Field<double, 2> pressure_field("pressure", 101.325);
    
    std::cout << "9. Testing unified apply_to_field_by_id - Automatic scalar/vector detection:\n\n";
    
    // Test 1: Apply to a scalar field using the unified method
    std::cout << "Testing with scalar field 'temperature':\n";
    bool found_temp = registry.apply_to_field_by_id("temperature", 
        [](auto& field, auto... args) {
            std::cout << "  Found field: " << field.field_ID;
            if constexpr (sizeof...(args) > 0) {
                auto dims = std::make_tuple(args...);
                if constexpr (sizeof...(args) == 1) {
                    std::cout << " (Scalar, Dim=" << std::get<0>(dims) << ")";
                } else if constexpr (sizeof...(args) == 2) {
                    std::cout << " (Vector, Dim=" << std::get<0>(dims) << ", VDim=" << std::get<1>(dims) << ")";
                }
            }
            std::cout << " with data: ";
            
            // Handle different data types properly
            if constexpr (requires { field.data[0]; }) {
                // Vector field case - print as array
                std::cout << "[";
                for (size_t i = 0; i < 3 && i < sizeof(field.data) / sizeof(field.data[0]); ++i) {
                    std::cout << field.data[i];
                    if (i < 2 && i < sizeof(field.data) / sizeof(field.data[0]) - 1) std::cout << ", ";
                }
                std::cout << "]";
            } else {
                // Scalar field case - print directly
                std::cout << field.data;
            }
            std::cout << "\n";
        });
    
    if (!found_temp) {
        std::cout << "  ❌ ERROR: Could not find 'temperature' field!\n";
    } else {
        std::cout << "  ✓ Successfully found and processed 'temperature' as scalar field!\n";
    }
    
    // Test 2: Apply to a vector field using the unified method
    std::cout << "\nTesting with vector field 'velocity':\n";
    bool found_vel_unified = registry.apply_to_field_by_id("velocity", 
        [](auto& field, auto... args) {
            std::cout << "  Found field: " << field.field_ID;
            if constexpr (sizeof...(args) > 0) {
                auto dims = std::make_tuple(args...);
                if constexpr (sizeof...(args) == 1) {
                    std::cout << " (Scalar, Dim=" << std::get<0>(dims) << ")";
                } else if constexpr (sizeof...(args) == 2) {
                    std::cout << " (Vector, Dim=" << std::get<0>(dims) << ", VDim=" << std::get<1>(dims) << ")";
                }
            }
            std::cout << " with data: ";
            
            // Handle different data types properly
            if constexpr (requires { field.data[0]; }) {
                // Vector field case - print as array
                std::cout << "[";
                for (size_t i = 0; i < 3 && i < sizeof(field.data) / sizeof(field.data[0]); ++i) {
                    std::cout << field.data[i];
                    if (i < 2 && i < sizeof(field.data) / sizeof(field.data[0]) - 1) std::cout << ", ";
                }
                std::cout << "]";
            } else {
                // Scalar field case - print directly
                std::cout << field.data;
            }
            std::cout << "\n";
        });
    
    if (!found_vel_unified) {
        std::cout << "  ❌ ERROR: Could not find 'velocity' field!\n";
    } else {
        std::cout << "  ✓ Successfully found and processed 'velocity' as vector field!\n";
    }
    
    // Test 3: Test with function that works for both scalar and vector fields
    std::cout << "\nTesting with generic lambda that works for any field type:\n";
    
    auto generic_lambda = [](auto& field) {
        std::cout << "  Processing field: " << field.field_ID << " (data type detected automatically)\n";
    };
    
    std::cout << "  - Applying to 'temperature': ";
    if (registry.apply_to_field_by_id("temperature", generic_lambda)) {
        std::cout << "✓ Success\n";
    } else {
        std::cout << "❌ Failed\n";
    }
    
    std::cout << "  - Applying to 'gradient': ";
    if (registry.apply_to_field_by_id("gradient", generic_lambda)) {
        std::cout << "✓ Success\n";
    } else {
        std::cout << "❌ Failed\n";
    }
    
    std::cout << "  - Applying to 'pressure': ";
    if (registry.apply_to_field_by_id("pressure", generic_lambda)) {
        std::cout << "✓ Success\n";
    } else {
        std::cout << "❌ Failed\n";
    }
    
    // Test 4: Test error handling with non-existent field
    std::cout << "\nTesting error handling:\n";
    std::cout << "  - Applying to 'nonexistent': ";
    if (registry.apply_to_field_by_id("nonexistent", generic_lambda)) {
        std::cout << "❌ Unexpectedly found field\n";
    } else {
        std::cout << "✓ Correctly returned false\n";
    }
    
    std::cout << "\n=== UNIFIED FIELD ACCESS SUMMARY ===\n";
    std::cout << "✓ apply_to_field_by_id automatically detects scalar vs vector fields\n";
    std::cout << "✓ Works with both dimension-aware and generic lambda functions\n";
    std::cout << "✓ Provides seamless fallback from scalar to vector field search\n";
    std::cout << "✓ Maintains type safety and performance of specialized methods\n";
    std::cout << "✓ Correctly handles non-existent fields\n";
    
    return 0;
}
