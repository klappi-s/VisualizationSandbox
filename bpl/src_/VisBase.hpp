#pragma once
#include "VisBase.h"
#include <typeinfo>
#include <type_traits>

// Forward declare vec to avoid circular dependency
template<typename T, unsigned Dim>
struct vec;

// Helper template to detect vec types
template<typename T>
struct is_vec : std::false_type {};

template<typename ScalarType, unsigned VDim>
struct is_vec<vec<ScalarType, VDim>> : std::true_type {
    using scalar_type = ScalarType;
    static constexpr unsigned vdim = VDim;
};

// Template function implementation for type registration
template<typename T, unsigned Dim>
void VisBase::registerFieldType(Field_b* field_ptr) {
    if (!field_ptr) return;
    
    // Determine if this is a vector type by checking if T is a vec<>
    bool is_vector = is_vec<T>::value;
    unsigned vdim = 0;
    size_t scalar_type_hash = 0;
    
    if constexpr (is_vec<T>::value) {
        vdim = is_vec<T>::vdim;
        scalar_type_hash = typeid(typename is_vec<T>::scalar_type).hash_code();
    } else {
        // For scalar types, the scalar_type_hash is the same as type_hash
        scalar_type_hash = typeid(T).hash_code();
    }
    
    // Store type information
    size_t type_hash = typeid(T).hash_code();
    
    TypeInfo info = std::make_tuple(type_hash, Dim, vdim, is_vector, scalar_type_hash);
    
    // Store type info parallel to sf_c
    sf_type_info.push_back(info);
    
    // Register the data extractor and field creator for this specific type
    // Only register if not already present
    if (data_extractors.find(type_hash) == data_extractors.end()) {
        // Create the data extractor lambda for this specific type
        DataExtractor extractor = [](Field_b* field_ptr) -> std::any {
            auto* typed_field = static_cast<Field<T, Dim>*>(field_ptr);
            return std::any(typed_field->data);
        };
        
        // Create the field creator lambda for this specific type
        FieldCreator creator = [](const std::any& data, const std::string& id) -> std::unique_ptr<Field_b> {
            T typed_data = std::any_cast<T>(data);
            return std::make_unique<Field<T, Dim>>(id, typed_data);
        };
        
        // Register both functions
        registerTypeHandlers(type_hash, extractor, creator);
    }
}