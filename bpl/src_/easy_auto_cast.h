/*
 * SIMPLEST APPROACH: AUTO-CAST FUNCTION
 * 
 * This is the most straightforward way to get typed pointers without
 * knowing template parameters and without visitor patterns.
 */

#pragma once
#include "field.h"
#include "VisBase.h"

// =====================================================================
// ULTRA-SIMPLE AUTO-CAST FUNCTION
// =====================================================================

namespace EasyFieldAccess {

// Single function that returns void* pointing to correctly typed field
inline void* getTypedField(const std::string& field_id) {
    Field_b* base = VisBase::findFieldByID(field_id);
    if (!base) return nullptr;
    
    // Get type information from VisBase registry
    auto it = VisBase::field_id_map.find(field_id);
    if (it == VisBase::field_id_map.end() || it->second >= VisBase::sf_type_info.size()) {
        return nullptr;
    }
    
    const auto& type_info = VisBase::sf_type_info[it->second];
    auto [type_hash, dim, vdim, is_vector, scalar_hash] = type_info;
    
    // Auto-detect and cast based on stored type information
    if (!is_vector) {
        // Scalar fields
        if (scalar_hash == typeid(double).hash_code()) {
            switch (dim) {
                case 1: return dynamic_cast<Field<double, 1>*>(base);
                case 2: return dynamic_cast<Field<double, 2>*>(base);
                case 3: return dynamic_cast<Field<double, 3>*>(base);
                case 4: return dynamic_cast<Field<double, 4>*>(base);
            }
        } else if (scalar_hash == typeid(float).hash_code()) {
            switch (dim) {
                case 1: return dynamic_cast<Field<float, 1>*>(base);
                case 2: return dynamic_cast<Field<float, 2>*>(base);
                case 3: return dynamic_cast<Field<float, 3>*>(base);
                case 4: return dynamic_cast<Field<float, 4>*>(base);
            }
        } else if (scalar_hash == typeid(int).hash_code()) {
            switch (dim) {
                case 1: return dynamic_cast<Field<int, 1>*>(base);
                case 2: return dynamic_cast<Field<int, 2>*>(base);
                case 3: return dynamic_cast<Field<int, 3>*>(base);
                case 4: return dynamic_cast<Field<int, 4>*>(base);
            }
        }
    } else {
        // Vector fields - double vectors
        if (scalar_hash == typeid(double).hash_code()) {
            if (vdim == 1) {
                switch (dim) {
                    case 1: return dynamic_cast<Field<vec<double, 1>, 1>*>(base);
                    case 2: return dynamic_cast<Field<vec<double, 1>, 2>*>(base);
                    case 3: return dynamic_cast<Field<vec<double, 1>, 3>*>(base);
                    case 4: return dynamic_cast<Field<vec<double, 1>, 4>*>(base);
                }
            } else if (vdim == 2) {
                switch (dim) {
                    case 1: return dynamic_cast<Field<vec<double, 2>, 1>*>(base);
                    case 2: return dynamic_cast<Field<vec<double, 2>, 2>*>(base);
                    case 3: return dynamic_cast<Field<vec<double, 2>, 3>*>(base);
                    case 4: return dynamic_cast<Field<vec<double, 2>, 4>*>(base);
                }
            } else if (vdim == 3) {
                switch (dim) {
                    case 1: return dynamic_cast<Field<vec<double, 3>, 1>*>(base);
                    case 2: return dynamic_cast<Field<vec<double, 3>, 2>*>(base);
                    case 3: return dynamic_cast<Field<vec<double, 3>, 3>*>(base);
                    case 4: return dynamic_cast<Field<vec<double, 3>, 4>*>(base);
                }
            } else if (vdim == 4) {
                switch (dim) {
                    case 1: return dynamic_cast<Field<vec<double, 4>, 1>*>(base);
                    case 2: return dynamic_cast<Field<vec<double, 4>, 2>*>(base);
                    case 3: return dynamic_cast<Field<vec<double, 4>, 3>*>(base);
                    case 4: return dynamic_cast<Field<vec<double, 4>, 4>*>(base);
                }
            }
        }
        // Add float and int vector types as needed...
    }
    
    return nullptr;
}

// Helper function to get type information about the field
inline std::string getFieldTypeInfo(const std::string& field_id) {
    auto it = VisBase::field_id_map.find(field_id);
    if (it == VisBase::field_id_map.end() || it->second >= VisBase::sf_type_info.size()) {
        return "Field not found";
    }
    
    const auto& type_info = VisBase::sf_type_info[it->second];
    auto [type_hash, dim, vdim, is_vector, scalar_hash] = type_info;
    
    std::string scalar_name;
    if (scalar_hash == typeid(double).hash_code()) scalar_name = "double";
    else if (scalar_hash == typeid(float).hash_code()) scalar_name = "float";
    else if (scalar_hash == typeid(int).hash_code()) scalar_name = "int";
    else scalar_name = "unknown";
    
    if (is_vector) {
        return "Field<vec<" + scalar_name + ", " + std::to_string(vdim) + ">, " + std::to_string(dim) + ">";
    } else {
        return "Field<" + scalar_name + ", " + std::to_string(dim) + ">";
    }
}

// Ultra-convenient template wrapper
template<typename T>
inline T* getFieldAs(const std::string& field_id) {
    void* ptr = getTypedField(field_id);
    return static_cast<T*>(ptr);
}

} // namespace EasyFieldAccess

/*
 * USAGE - SUPER SIMPLE:
 * 
 * #include "easy_auto_cast.h"
 * 
 * // Method 1: Get void* and cast yourself
 * void* field_ptr = EasyFieldAccess::getTypedField("my_field");
 * if (field_ptr) {
 *     // field_ptr is already the correct Field<T, Dim>* type
 *     // You just need to cast it to the right type
 *     // Use getFieldTypeInfo() to see what type it is
 *     std::cout << "Type: " << EasyFieldAccess::getFieldTypeInfo("my_field") << std::endl;
 * }
 * 
 * // Method 2: Template helper (if you suspect the type)
 * auto* typed_field = EasyFieldAccess::getFieldAs<Field<double, 3>>("my_field");
 * if (typed_field) {
 *     typed_field->data[0] = 42.0;  // Direct access to all methods!
 * }
 * 
 * // Method 3: Check type first, then cast
 * std::string type_info = EasyFieldAccess::getFieldTypeInfo("my_field");
 * std::cout << "Field type: " << type_info << std::endl;
 * 
 * if (type_info.find("Field<double, 3>") != std::string::npos) {
 *     auto* field = EasyFieldAccess::getFieldAs<Field<double, 3>>("my_field");
 *     // Use field...
 * } else if (type_info.find("Field<vec<double, 3>, 2>") != std::string::npos) {
 *     auto* field = EasyFieldAccess::getFieldAs<Field<vec<double, 3>, 2>>("my_field");
 *     // Use field...
 * }
 * 
 * // This is the simplest possible way - no visitor pattern, no complex templates,
 * // no knowing template parameters beforehand!
 */
