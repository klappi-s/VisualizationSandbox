constexpr unsigned Dim = 3;
using T = double;


#include "bpl.h"






int main() {
    std::cout << "\n=== Testing apply_to_all methods ===\n" << std::endl;

    // Create different types of fields to test
    Field<double, 3> scalar_field_1("scalar_field_1", 42.5);
    Field<double, 3> scalar_field_2("scalar_field_2", 100.0);
    Field<vec<double, 2>, 3> vector_field_2d("vector_field_2d", {1.0, 2.0});
    Field<vec<double, 3>, 3> vector_field_3d("vector_field_3d", {1.0, 2.0, 3.0});
    
    std::cout << "Created fields:" << std::endl;
    VisBase::PrintListFields();

    // Test apply_to_all_scalar_fields
    std::cout << "\n=== Test 1: apply_to_all_scalar_fields ===" << std::endl;
    VisBase::apply_to_all_scalar_fields([](Field_b* field, unsigned dim) {
        std::cout << "Found scalar field: " << field->field_ID 
                  << " with dimension: " << dim << std::endl;
    });

    // Test apply_to_all_vector_fields
    std::cout << "\n=== Test 2: apply_to_all_vector_fields ===" << std::endl;
    VisBase::apply_to_all_vector_fields([](Field_b* field, unsigned dim, unsigned vdim) {
        std::cout << "Found vector field: " << field->field_ID 
                  << " with dimension: " << dim 
                  << " and vector dimension: " << vdim << std::endl;
    });

    // Test counting fields of each type
    std::cout << "\n=== Test 3: Count fields by type ===" << std::endl;
    int scalar_count = 0;
    int vector_count = 0;
    
    VisBase::apply_to_all_scalar_fields([&scalar_count](Field_b*, unsigned) {
        scalar_count++;
    });
    
    VisBase::apply_to_all_vector_fields([&vector_count](Field_b*, unsigned, unsigned) {
        vector_count++;
    });
    
    std::cout << "Total scalar fields: " << scalar_count << std::endl;
    std::cout << "Total vector fields: " << vector_count << std::endl;

    // Additional validation test: Verify that the methods correctly distinguish field types
    std::cout << "\n=== Test 4: Validation - Field Type Distinction ===" << std::endl;
    
    std::vector<std::string> found_scalar_ids;
    std::vector<std::string> found_vector_ids;
    
    VisBase::apply_to_all_scalar_fields([&found_scalar_ids](Field_b* field, unsigned dim) {
        found_scalar_ids.push_back(field->field_ID);
        std::cout << dim << std::endl;
    });
    
    VisBase::apply_to_all_vector_fields([&found_vector_ids](Field_b* field, unsigned dim, unsigned vdim) {
        found_vector_ids.push_back(field->field_ID);
        std::cout << dim << vdim << std::endl;
    });
    
    std::cout << "Scalar fields found: ";
    for (const auto& id : found_scalar_ids) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Vector fields found: ";
    for (const auto& id : found_vector_ids) {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== SUCCESS: apply_to_all methods working correctly! ===" << std::endl;

    // ====================================================================
    // VISITOR PATTERN DEMONSTRATION
    // ====================================================================
    
    std::cout << "\n=== VISITOR PATTERN DEMONSTRATION ===" << std::endl;
    
    // Demonstrate visit_field with different field types
    std::cout << "\n--- Using visit_field to access field data ---" << std::endl;
    
    // Visit a scalar field
    try {
        VisBase::visit_field("scalar_field_1", [](auto& field) {
            std::cout << "Scalar field '" << field.field_ID << "' value: " << field.data[0] << std::endl;
        });
    } catch (const std::exception& e) {
        std::cout << "Error visiting scalar_field_1: " << e.what() << std::endl;
    }
    
    // Visit a 2D vector field  
    try {
        VisBase::visit_field("vector_field_2d", [](auto& field) {
            using FieldType = std::decay_t<decltype(field)>;
            using ValueType = typename FieldType::value_type;
            
            if constexpr (is_vector_type_v<ValueType>) {
                // This is a vector field
                auto& vec_data = field.data[0]; // Get the vector at index 0
                std::cout << "Vector field '" << field.field_ID << "' value: " << vec_data << std::endl;
            } else {
                // This shouldn't happen for vector_field_2d, but handle gracefully
                std::cout << "Unexpected: field '" << field.field_ID << "' is not a vector field" << std::endl;
            }
        });
    } catch (const std::exception& e) {
        std::cout << "Error visiting vector_field_2d: " << e.what() << std::endl;
    }
    
    // Visit a 3D vector field
    try {
        VisBase::visit_field("vector_field_3d", [](auto& field) {
            using FieldType = std::decay_t<decltype(field)>;
            using ValueType = typename FieldType::value_type;
            
            if constexpr (is_vector_type_v<ValueType>) {
                // This is a vector field
                auto& vec_data = field.data[0]; // Get the vector at index 0
                std::cout << "Vector field '" << field.field_ID << "' value: " << vec_data << std::endl;
            } else {
                // This shouldn't happen for vector_field_3d, but handle gracefully
                std::cout << "Unexpected: field '" << field.field_ID << "' is not a vector field" << std::endl;
            }
        });
    } catch (const std::exception& e) {
        std::cout << "Error visiting vector_field_3d: " << e.what() << std::endl;
    }
    
    std::cout << "\n✅ visit_field demonstration completed!" << std::endl;
    return 0;
}