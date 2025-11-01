#include "auto_typed_pointer_patterns.h"
#include "field.h"
#include "VisBase.h"

// =====================================================================
// IMPLEMENTATION OF AUTO-DETECTING WRAPPER
// =====================================================================

void* AutoFieldPtr::detectAndCast() const {
    if (!field_ptr) return nullptr;
    
    // Use stored type information from VisBase registry
    size_t type_hash = field_ptr->getTypeHash();
    size_t dim = field_ptr->getDim();
    
    // Try all known scalar types
    if (type_hash == typeid(double).hash_code()) {
        switch (dim) {
            case 1: return dynamic_cast<Field<double, 1>*>(field_ptr);
            case 2: return dynamic_cast<Field<double, 2>*>(field_ptr);
            case 3: return dynamic_cast<Field<double, 3>*>(field_ptr);
            case 4: return dynamic_cast<Field<double, 4>*>(field_ptr);
        }
    } else if (type_hash == typeid(float).hash_code()) {
        switch (dim) {
            case 1: return dynamic_cast<Field<float, 1>*>(field_ptr);
            case 2: return dynamic_cast<Field<float, 2>*>(field_ptr);
            case 3: return dynamic_cast<Field<float, 3>*>(field_ptr);
            case 4: return dynamic_cast<Field<float, 4>*>(field_ptr);
        }
    } else if (type_hash == typeid(int).hash_code()) {
        switch (dim) {
            case 1: return dynamic_cast<Field<int, 1>*>(field_ptr);
            case 2: return dynamic_cast<Field<int, 2>*>(field_ptr);
            case 3: return dynamic_cast<Field<int, 3>*>(field_ptr);
            case 4: return dynamic_cast<Field<int, 4>*>(field_ptr);
        }
    }
    
    // Try vector types - we need to check if this is a vector field
    // This requires access to VisBase type information
    auto it = VisBase::field_id_map.find(field_ptr->field_ID);
    if (it != VisBase::field_id_map.end() && it->second < VisBase::sf_type_info.size()) {
        const auto& type_info = VisBase::sf_type_info[it->second];
        auto [full_type_hash, stored_dim, vdim, is_vector, scalar_hash] = type_info;
        
        if (is_vector) {
            // Try vector types based on scalar type and dimensions
            if (scalar_hash == typeid(double).hash_code()) {
                // Try different vector dimensions
                if (vdim == 1) {
                    switch (stored_dim) {
                        case 1: return dynamic_cast<Field<vec<double, 1>, 1>*>(field_ptr);
                        case 2: return dynamic_cast<Field<vec<double, 1>, 2>*>(field_ptr);
                        case 3: return dynamic_cast<Field<vec<double, 1>, 3>*>(field_ptr);
                        case 4: return dynamic_cast<Field<vec<double, 1>, 4>*>(field_ptr);
                    }
                } else if (vdim == 2) {
                    switch (stored_dim) {
                        case 1: return dynamic_cast<Field<vec<double, 2>, 1>*>(field_ptr);
                        case 2: return dynamic_cast<Field<vec<double, 2>, 2>*>(field_ptr);
                        case 3: return dynamic_cast<Field<vec<double, 2>, 3>*>(field_ptr);
                        case 4: return dynamic_cast<Field<vec<double, 2>, 4>*>(field_ptr);
                    }
                } else if (vdim == 3) {
                    switch (stored_dim) {
                        case 1: return dynamic_cast<Field<vec<double, 3>, 1>*>(field_ptr);
                        case 2: return dynamic_cast<Field<vec<double, 3>, 2>*>(field_ptr);
                        case 3: return dynamic_cast<Field<vec<double, 3>, 3>*>(field_ptr);
                        case 4: return dynamic_cast<Field<vec<double, 3>, 4>*>(field_ptr);
                    }
                } else if (vdim == 4) {
                    switch (stored_dim) {
                        case 1: return dynamic_cast<Field<vec<double, 4>, 1>*>(field_ptr);
                        case 2: return dynamic_cast<Field<vec<double, 4>, 2>*>(field_ptr);
                        case 3: return dynamic_cast<Field<vec<double, 4>, 3>*>(field_ptr);
                        case 4: return dynamic_cast<Field<vec<double, 4>, 4>*>(field_ptr);
                    }
                }
            }
            // Add similar blocks for float and int vector types...
        }
    }
    
    return nullptr;
}

std::string AutoFieldPtr::getTypeString() const {
    if (!field_ptr) return "null";
    
    auto it = VisBase::field_id_map.find(field_ptr->field_ID);
    if (it != VisBase::field_id_map.end() && it->second < VisBase::sf_type_info.size()) {
        const auto& type_info = VisBase::sf_type_info[it->second];
        auto [full_type_hash, stored_dim, vdim, is_vector, scalar_hash] = type_info;
        
        std::string scalar_name;
        if (scalar_hash == typeid(double).hash_code()) scalar_name = "double";
        else if (scalar_hash == typeid(float).hash_code()) scalar_name = "float";
        else if (scalar_hash == typeid(int).hash_code()) scalar_name = "int";
        else scalar_name = "unknown";
        
        if (is_vector) {
            return "Field<vec<" + scalar_name + ", " + std::to_string(vdim) + ">, " + std::to_string(stored_dim) + ">";
        } else {
            return "Field<" + scalar_name + ", " + std::to_string(stored_dim) + ">";
        }
    }
    
    return "unknown";
}

bool AutoFieldPtr::isScalar() const {
    if (!field_ptr) return false;
    
    auto it = VisBase::field_id_map.find(field_ptr->field_ID);
    if (it != VisBase::field_id_map.end() && it->second < VisBase::sf_type_info.size()) {
        const auto& type_info = VisBase::sf_type_info[it->second];
        return !std::get<3>(type_info); // !is_vector
    }
    return false;
}

bool AutoFieldPtr::isVector() const {
    if (!field_ptr) return false;
    
    auto it = VisBase::field_id_map.find(field_ptr->field_ID);
    if (it != VisBase::field_id_map.end() && it->second < VisBase::sf_type_info.size()) {
        const auto& type_info = VisBase::sf_type_info[it->second];
        return std::get<3>(type_info); // is_vector
    }
    return false;
}

// =====================================================================
// VISBASE IMPLEMENTATIONS
// =====================================================================

TypedFieldHandle VisBase::getAutoFieldHandle(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        return TypedFieldHandle(nullptr, 0, 0, 0, false, 0, nullptr);
    }
    
    size_t index = it->second;
    const TypeInfo& info = sf_type_info[index];
    auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
    Field_b* field_ptr = sf_c[index];
    
    // Create cast function based on type information
    std::function<void*(Field_b*)> cast_function;
    
    if (!is_vector) {
        // Scalar field casting
        if (scalar_type_hash == typeid(double).hash_code()) {
            switch (dim) {
                case 1: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<double, 1>*>(f); }; break;
                case 2: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<double, 2>*>(f); }; break;
                case 3: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<double, 3>*>(f); }; break;
                case 4: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<double, 4>*>(f); }; break;
            }
        } else if (scalar_type_hash == typeid(float).hash_code()) {
            switch (dim) {
                case 1: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<float, 1>*>(f); }; break;
                case 2: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<float, 2>*>(f); }; break;
                case 3: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<float, 3>*>(f); }; break;
                case 4: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<float, 4>*>(f); }; break;
            }
        } else if (scalar_type_hash == typeid(int).hash_code()) {
            switch (dim) {
                case 1: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<int, 1>*>(f); }; break;
                case 2: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<int, 2>*>(f); }; break;
                case 3: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<int, 3>*>(f); }; break;
                case 4: cast_function = [](Field_b* f) -> void* { return dynamic_cast<Field<int, 4>*>(f); }; break;
            }
        }
    } else {
        // Vector field casting - example for double vectors
        if (scalar_type_hash == typeid(double).hash_code() && vdim == 3 && dim == 2) {
            cast_function = [](Field_b* f) -> void* { 
                return dynamic_cast<Field<vec<double, 3>, 2>*>(f); 
            };
        }
        // Add more vector type combinations as needed...
    }
    
    return TypedFieldHandle(field_ptr, type_hash, dim, vdim, is_vector, scalar_type_hash, cast_function);
}

// =====================================================================
// SIMPLE USAGE EXAMPLE FUNCTIONS
// =====================================================================

void demonstrateAutoTypedPointers() {
    std::cout << "\n=== AUTO TYPED POINTER DEMONSTRATION ===" << std::endl;
    
    // Create some fields for testing
    Field<double, 3> scalar_field("test_scalar", 42.0);
    Field<vec<double, 3>, 2> vector_field("test_vector");
    
    // Method 1: TypedFieldHandle
    std::cout << "\n--- Method 1: TypedFieldHandle ---" << std::endl;
    auto handle = VisBase::getAutoFieldHandle("test_scalar");
    if (handle.isValid()) {
        std::cout << "Field found! Is vector: " << handle.isVector() << std::endl;
        auto* typed = handle.as<Field<double, 3>>();
        if (typed) {
            std::cout << "Successfully cast to Field<double, 3>. Value: " << typed->data[0] << std::endl;
        }
    }
    
    // Method 2: AutoFieldPtr
    std::cout << "\n--- Method 2: AutoFieldPtr ---" << std::endl;
    auto auto_field = VisBase::getAutoField("test_vector");
    if (auto_field.isValid()) {
        std::cout << "Field type: " << auto_field.getTypeString() << std::endl;
        std::cout << "Is vector: " << auto_field.isVector() << std::endl;
        
        void* typed_ptr = auto_field.getAuto();
        if (typed_ptr) {
            // We know it's a vector field, so we can cast it
            auto* vector_field_ptr = static_cast<Field<vec<double, 3>, 2>*>(typed_ptr);
            std::cout << "Successfully auto-detected and cast vector field!" << std::endl;
        }
    }
    
    // Method 3: Macro-based
    std::cout << "\n--- Method 3: Macro-based ---" << std::endl;
    void* macro_result = VisBase::getAutoFieldPtr("test_scalar");
    if (macro_result) {
        std::cout << "Macro successfully found typed pointer!" << std::endl;
        // The pointer is already correctly typed, just need to cast it
    }
}
