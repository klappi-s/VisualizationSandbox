#pragma once
#include <iostream>
#include <string>
#include <any>
#include <typeinfo>
#include <vector>

#include "VIS_forward.h"

/*
 * CRTP Pattern for Type-Safe Current Object Access
 * 
 * This pattern allows returning correctly typed pointers to the current field
 * without needing explicit casting in the calling code.
 * 
 * Benefits:
 * - Type Safety: getTyped() returns exactly Field<T, Dim>*
 * - No Casting Needed: Direct access to all field methods
 * - Compile-Time Verification: Wrong types caught at compile time
 * - Performance: No virtual function overhead for typed access
 * - Clean API: auto* field = some_field.getTyped() is very readable
 */

// Base interface
class Field_b {
public:
    std::string field_ID;
    virtual ~Field_b() = default;
    
    // Virtual methods for type-erased access
    virtual std::any getData() const = 0;
    virtual size_t getTypeHash() const = 0;
    virtual size_t getDim() const = 0;
    
    // Pure virtual method for getting typed pointer
    virtual void* getTypedPtr() = 0;
    virtual const void* getTypedPtr() const = 0;
};

// CRTP implementation that provides typed pointer access
template<typename Derived>
class FieldCRTP : public Field_b {
public:
    // Return correctly typed pointer to current object
    Derived* getTyped() {
        return static_cast<Derived*>(this);
    }
    
    const Derived* getTyped() const {
        return static_cast<const Derived*>(this);
    }
    
    // Implement virtual interface
    void* getTypedPtr() override {
        return static_cast<void*>(static_cast<Derived*>(this));
    }
    
    const void* getTypedPtr() const override {
        return static_cast<const void*>(static_cast<const Derived*>(this));
    }
};

// Your field implementation using CRTP
template<typename T, unsigned Dim>
class Field : public FieldCRTP<Field<T, Dim>> {
public:
    using value_type = T;
    inline static int counter = 0;
    std::array<T, Dim> data;
    
    Field();
    Field(std::string name);
    Field(std::string name, T v);
    
    // Implement virtual methods from base class
    std::any getData() const override {
        return std::any(data);
    }
    
    size_t getTypeHash() const override {
        return typeid(T).hash_code();
    }
    
    size_t getDim() const override {
        return Dim;
    }
    
    // Direct typed access methods
    const std::array<T, Dim>& getDataArray() const { return data; }
    std::array<T, Dim>& getDataArray() { return data; }
};

// VisBase integration methods
class VisBase {
public:
    // Method 1: When you know the exact type
    template<typename T, unsigned Dim>
    static Field<T, Dim>* getTypedFieldPtr(const std::string& field_id) {
        Field_b* base_ptr = findFieldByID(field_id);
        if (!base_ptr) return nullptr;
        
        // Try dynamic cast to specific type
        if (auto* typed_ptr = dynamic_cast<Field<T, Dim>*>(base_ptr)) {
            return typed_ptr->getTyped();  // Returns Field<T, Dim>*
        }
        
        return nullptr;
    }
    
    // Auto-detection version using stored type info
    static Field_b* getTypedFieldPtr(const std::string& field_id) {
        auto it = field_id_map.find(field_id);
        if (it == field_id_map.end() || it->second >= sf_c.size()) {
            return nullptr;
        }
        
        Field_b* field_ptr = sf_c[it->second];
        
        // Use CRTP to get typed pointer through virtual interface
        return field_ptr;  // Base pointer, but can call getTyped() on derived
    }
    
    // Template visitor pattern with CRTP
    template<typename Visitor>
    static void visitFieldWithCRTP(const std::string& field_id, Visitor&& visitor) {
        Field_b* field = findFieldByID(field_id);
        if (!field) return;
        
        // Dispatch to correct type and use CRTP
        // ... type detection logic ...
        
        if (auto* typed = dynamic_cast<Field<double, 3>*>(field)) {
            visitor(typed->getTyped());  // Pass correctly typed pointer
        }
        // ... other types ...
    }
    
private:
    // These would be your existing VisBase members
    static std::vector<Field_b*> sf_c;
    static std::unordered_map<std::string, size_t> field_id_map;
    static Field_b* findFieldByID(const std::string& field_id);
};

/*
 * USAGE EXAMPLES:
 * 
 * // Create different field types
 * Field<double, 3> scalar_field("my_scalar", 42.0);
 * Field<vec<double, 3>, 2> vector_field("my_vector");
 * 
 * // Store as base pointers
 * std::vector<Field_b*> fields = {&scalar_field, &vector_field};
 * 
 * // Access with correct typing through CRTP
 * for (auto* field_base : fields) {
 *     std::cout << "Field ID: " << field_base->field_ID << std::endl;
 *     
 *     // Try to get as scalar field
 *     if (auto* scalar_field_ptr = dynamic_cast<Field<double, 3>*>(field_base)) {
 *         // Now we have the correctly typed pointer!
 *         auto typed_ptr = scalar_field_ptr->getTyped();  // Field<double, 3>*
 *         std::cout << "Scalar field value: " << typed_ptr->data[0] << std::endl;
 *         typed_ptr->data[0] *= 2.0;  // Can use all Field<double, 3> methods
 *     }
 *     
 *     // Try to get as vector field
 *     if (auto* vector_field_ptr = dynamic_cast<Field<vec<double, 3>, 2>*>(field_base)) {
 *         auto typed_ptr = vector_field_ptr->getTyped();  // Field<vec<double, 3>, 2>*
 *         std::cout << "Vector field: " << typed_ptr->data[0] << std::endl;
 *         typed_ptr->data[0].x() += 1.0;  // Can use all methods
 *     }
 * }
 * 
 * // Method 1: When you know the exact type
 * auto* typed_field = VisBase::getTypedFieldPtr<double, 3>("my_scalar");
 * if (typed_field) {
 *     // typed_field is Field<double, 3>*
 *     typed_field->data[0] = 100.0;
 *     std::cout << "Value: " << typed_field->getDataArray()[0] << std::endl;
 * }
 * 
 * // Method 2: Using CRTP on any field
 * Field_b* base_field = VisBase::findFieldByID("my_vector");
 * if (base_field) {
 *     // Cast to CRTP base to access getTyped()
 *     if (auto* crtp_field = dynamic_cast<FieldCRTP<Field<vec<double, 3>, 2>>*>(base_field)) {
 *         auto* typed_ptr = crtp_field->getTyped();  // Field<vec<double, 3>, 2>*
 *         typed_ptr->data[0].x() = 42.0;
 *     }
 * }
 * 
 * // Method 3: One-liner field access and modification
 * VisBase::visitFieldWithCRTP("scalar_field_2", [](auto* field) { 
 *     field->data[0] *= 2.0; 
 * });
 */
