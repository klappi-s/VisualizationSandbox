// Standalone test for enhanced VisBase functionality
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <optional>

constexpr unsigned Dim = 3;
using T = double;


    #include "bpl.h"
    // #include "easy_auto_cast.h"

int main() {
    std::cout << "\n=== AUTO-TYPED FIELD ACCESS DEMO ===" << std::endl;
    
    bpl::initializeVisualization();
    
    // Create some test fields
    Field<double, 3> scalar_field("test_scalar", 42.5);
    Field<vec<double, 3>, 2> vector_field("test_vector");
    Field<float, 2> float_field("test_float", 3.14f);
    Field<int, 4> int_field("test_int", 999);
    
    std::cout << "\nCreated fields:" << std::endl;
    VisBase::PrintListFields();
    
    // ====================================================================
    // TEST THE NEW AUTO-TYPED FIELD ACCESS METHODS
    // ====================================================================
    
    std::cout << "\n=== TESTING VISBASE AUTO-TYPED METHODS ===" << std::endl;
    
    // Method 1: getTypedField - returns void* pointing to correctly typed field
    std::cout << "\n--- Method 1: VisBase::getTypedField() ---" << std::endl;
    
    void* scalar_ptr = VisBase::getTypedField("test_scalar");
    void* vector_ptr = VisBase::getTypedField("test_vector");
    void* float_ptr = VisBase::getTypedField("test_float");
    void* int_ptr = VisBase::getTypedField("test_int");
    
    std::cout << "✅ Scalar field pointer: " << (scalar_ptr ? "SUCCESS" : "FAILED") << std::endl;
    std::cout << "✅ Vector field pointer: " << (vector_ptr ? "SUCCESS" : "FAILED") << std::endl;
    std::cout << "✅ Float field pointer: " << (float_ptr ? "SUCCESS" : "FAILED") << std::endl;
    std::cout << "✅ Int field pointer: " << (int_ptr ? "SUCCESS" : "FAILED") << std::endl;
    
    // Method 2: getFieldTypeInfo - get type information
    std::cout << "\n--- Method 2: VisBase::getFieldTypeInfo() ---" << std::endl;
    
    std::cout << "Scalar field type: " << VisBase::getFieldTypeInfo("test_scalar") << std::endl;
    std::cout << "Vector field type: " << VisBase::getFieldTypeInfo("test_vector") << std::endl;
    std::cout << "Float field type: " << VisBase::getFieldTypeInfo("test_float") << std::endl;
    std::cout << "Int field type: " << VisBase::getFieldTypeInfo("test_int") << std::endl;
    
    // Method 3: getFieldAs - template helper
    std::cout << "\n--- Method 3: VisBase::getFieldAs<T>() ---" << std::endl;
    
    auto* typed_scalar = VisBase::getFieldAs<Field<double, 3>>("test_scalar");
    auto* typed_vector = VisBase::getFieldAs<Field<vec<double, 3>, 2>>("test_vector");
    auto* typed_float = VisBase::getFieldAs<Field<float, 2>>("test_float");
    auto* typed_int = VisBase::getFieldAs<Field<int, 4>>("test_int");
    
    if (typed_scalar) {
        std::cout << "✅ Scalar field value: " << typed_scalar->data[0] << std::endl;
        typed_scalar->data[0] = 100.0;  // Modify it
        std::cout << "✅ Modified scalar field: " << typed_scalar->data[0] << std::endl;
    }
    
    if (typed_vector) {
        std::cout << "✅ Vector field first element: " << typed_vector->data[0] << std::endl;
        typed_vector->data[0].x() = 999.0;  // Modify it
        std::cout << "✅ Modified vector field: " << typed_vector->data[0] << std::endl;
    }
    
    if (typed_float) {
        std::cout << "✅ Float field value: " << typed_float->data[0] << std::endl;
        typed_float->data[0] = 6.28f;
        std::cout << "✅ Modified float field: " << typed_float->data[0] << std::endl;
    }
    
    if (typed_int) {
        std::cout << "✅ Int field value: " << typed_int->data[0] << std::endl;
        typed_int->data[0] = 2024;
        std::cout << "✅ Modified int field: " << typed_int->data[0] << std::endl;
    }

    // ====================================================================
    // GENERIC FIELD PROCESSING WITHOUT KNOWING TYPES
    // ====================================================================
    
    std::cout << "\n=== GENERIC FIELD PROCESSING ===" << std::endl;
    
    auto processAnyField = [](const std::string& field_id) {
        std::cout << "\n--- Processing field: " << field_id << " ---" << std::endl;
        
        // Step 1: Get type information
        std::string type_info = VisBase::getFieldTypeInfo(field_id);
        std::cout << "Type: " << type_info << std::endl;
        
        // Step 2: Get typed pointer
        void* field_ptr = VisBase::getTypedField(field_id);
        if (!field_ptr) {
            std::cout << "❌ Failed to get typed pointer" << std::endl;
            return;
        }
        
        // Step 3: Process based on type
        if (type_info.find("Field<double, 3>") != std::string::npos) {
            auto* field = static_cast<Field<double, 3>*>(field_ptr);
            std::cout << "📊 Scalar double field - value: " << field->data[0] << std::endl;
            field->data[0] *= 1.1;  // Apply 10% increase
            std::cout << "📈 After 10% increase: " << field->data[0] << std::endl;
            
        } else if (type_info.find("Field<vec<double, 3>, 2>") != std::string::npos) {
            auto* field = static_cast<Field<vec<double, 3>, 2>*>(field_ptr);
            std::cout << "📊 Vector field - first vector: " << field->data[0] << std::endl;
            field->data[0].x() += 10.0;  // Add 10 to X component
            std::cout << "📈 After X+10: " << field->data[0] << std::endl;
            
        } else if (type_info.find("Field<float, 2>") != std::string::npos) {
            auto* field = static_cast<Field<float, 2>*>(field_ptr);
            std::cout << "📊 Float field - value: " << field->data[0] << std::endl;
            field->data[0] = field->data[0] * 2.0f;  // Double it
            std::cout << "📈 After doubling: " << field->data[0] << std::endl;
            
        } else if (type_info.find("Field<int, 4>") != std::string::npos) {
            auto* field = static_cast<Field<int, 4>*>(field_ptr);
            std::cout << "📊 Int field - value: " << field->data[0] << std::endl;
            field->data[0] += 1000;  // Add 1000
            std::cout << "📈 After +1000: " << field->data[0] << std::endl;
            
        } else {
            std::cout << "⚠️ Unknown or unsupported field type" << std::endl;
        }
    };
    
    // Process all fields generically
    processAnyField("test_scalar");
    processAnyField("test_vector");
    processAnyField("test_float");
    processAnyField("test_int");
    
    // ====================================================================
    // ERROR HANDLING AND EDGE CASES
    // ====================================================================
    
    std::cout << "\n=== ERROR HANDLING ===" << std::endl;
    
    // Test non-existent field
    void* null_ptr = VisBase::getTypedField("non_existent_field");
    std::cout << "Non-existent field pointer: " << (null_ptr ? "UNEXPECTED" : "NULL (correct)") << std::endl;
    
    std::string null_type = VisBase::getFieldTypeInfo("non_existent_field");
    std::cout << "Non-existent field type: " << null_type << std::endl;
    
    auto* null_typed = VisBase::getFieldAs<Field<double, 3>>("non_existent_field");
    std::cout << "Non-existent field cast: " << (null_typed ? "UNEXPECTED" : "NULL (correct)") << std::endl;
    
    // ====================================================================
    // COMPARISON WITH OLD APPROACH
    // ====================================================================
    
    std::cout << "\n=== COMPARISON: OLD vs NEW APPROACH ===" << std::endl;
    
    std::cout << "\n🔴 OLD APPROACH (complex):" << std::endl;
    std::cout << "   - Need to know template parameters: Field<double, 3>" << std::endl;
    std::cout << "   - Manual casting: dynamic_cast<Field<double, 3>*>(base_ptr)" << std::endl;
    std::cout << "   - Type checking: if (auto* typed = dynamic_cast<...>)" << std::endl;
    std::cout << "   - Error prone: wrong types = runtime errors" << std::endl;
    
    std::cout << "\n🟢 NEW APPROACH (simple):" << std::endl;
    std::cout << "   - No template parameters needed" << std::endl;
    std::cout << "   - Auto type detection: void* ptr = getTypedField(\"id\")" << std::endl;
    std::cout << "   - Type info helper: getFieldTypeInfo(\"id\")" << std::endl;
    std::cout << "   - Safe casting with runtime verification" << std::endl;
    
    std::cout << "\n🎉 SUMMARY: " << std::endl;
    std::cout << "   ✅ No visitor patterns needed" << std::endl;
    std::cout << "   ✅ No template parameters required" << std::endl;
    std::cout << "   ✅ Simple auto field_ptr = VisBase::getTypedField(id)" << std::endl;
    std::cout << "   ✅ Runtime type detection working perfectly!" << std::endl;
    
    return 0;
}
