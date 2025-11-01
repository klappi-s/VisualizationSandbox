#pragma once
#include "VisBase.h"
#include <typeinfo>
#include <algorithm>

// Type configuration initialization
template<typename T, unsigned Dim>
void VisBase::initializeTypeConfig() {
    if (!type_config_set) {
        registered_scalar_type_hash = typeid(T).hash_code();
        registered_dim = Dim;
        type_config_set = true;
        std::cout << "Registry initialized for type: " << typeid(T).name() 
                  << ", Dim: " << Dim << std::endl;
    }
}

// Scalar field type validation
template<typename T, unsigned Dim>
void VisBase::validateScalarFieldType() {
    initializeTypeConfig<T, Dim>();
    
    if (typeid(T).hash_code() != registered_scalar_type_hash || Dim != registered_dim) {
        throw std::runtime_error(
            "Type mismatch: Cannot register Field<" + std::string(typeid(T).name()) + 
            "," + std::to_string(Dim) + ">. Registry is configured for type: " + 
            std::string(typeid(typename std::conditional<true, T, void>::type).name()) + 
            ", Dim: " + std::to_string(registered_dim)
        );
    }
}

// Vector field type validation  
template<typename T, unsigned Dim, unsigned VDim>
void VisBase::validateVectorFieldType() {
    // For vector fields, we expect T to be vec<ScalarType, VDim>
    // where ScalarType matches the registered scalar type
    if constexpr (is_vec<T>::value) {
        using ScalarType = typename is_vec<T>::scalar_type;
        initializeTypeConfig<ScalarType, Dim>();
        
        if (typeid(ScalarType).hash_code() != registered_scalar_type_hash || Dim != registered_dim) {
            throw std::runtime_error(
                "Type mismatch: Cannot register vector Field<vec<" + std::string(typeid(ScalarType).name()) + 
                "," + std::to_string(VDim) + ">," + std::to_string(Dim) + 
                ">. Registry is configured for scalar type: " + 
                std::string(typeid(typename std::conditional<true, ScalarType, void>::type).name()) + 
                ", Dim: " + std::to_string(registered_dim)
            );
        }
    } else {
        throw std::runtime_error(
            "Invalid vector field type: Expected vec<ScalarType, VDim> but got " + 
            std::string(typeid(T).name())
        );
    }
}

// Particle type validation
template<typename T, unsigned Dim>
void VisBase::validateParticleType() {
    initializeTypeConfig<T, Dim>();
    
    if (typeid(T).hash_code() != registered_scalar_type_hash || Dim != registered_dim) {
        throw std::runtime_error(
            "Type mismatch: Cannot register ParticleBase<" + std::string(typeid(T).name()) + 
            "," + std::to_string(Dim) + ">. Registry is configured for type: " + 
            std::string(typeid(typename std::conditional<true, T, void>::type).name()) + 
            ", Dim: " + std::to_string(registered_dim)
        );
    }
}