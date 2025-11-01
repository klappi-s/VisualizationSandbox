#pragma once
#include "VisBase.h"
#include "field.h"
#include "particle.h"
#include <typeinfo>
#include <algorithm>

// Type configuration initialization
template<typename T, unsigned Dim>
void VisBase::initializeTypeConfig() {
    if (!type_config_set) {
        registered_scalar_type_hash = typeid(T).hash_code();
        registered_dim = Dim;
        type_config_set = true;
        
        // Lazily set configured_registry upon first access through getConfiguredRegistry()
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
    // where ScalarType matches the registered scalar type AND VDim == Dim
    if constexpr (is_vec<T>::value) {
        using ScalarType = typename is_vec<T>::scalar_type;
        initializeTypeConfig<ScalarType, Dim>();
        
        // Check that VDim matches Dim
        if (VDim != Dim) {
            throw std::runtime_error(
                "Vector dimension mismatch: Cannot register vector Field<vec<" + std::string(typeid(ScalarType).name()) + 
                "," + std::to_string(VDim) + ">," + std::to_string(Dim) + 
                ">. Vector dimension (" + std::to_string(VDim) + 
                ") must match spatial dimension (" + std::to_string(Dim) + ")"
            );
        }
        
        // Check scalar type and spatial dimension
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

// TypedRegistry static member definitions
template<typename T, unsigned Dim>
TypedRegistry<T, Dim>* TypedRegistry<T, Dim>::instance = nullptr;

// TypedRegistry method implementations
template<typename T, unsigned Dim>
Field<T, Dim>* TypedRegistry<T, Dim>::findScalarFieldByID(const std::string& field_id) {
    for (Field<T, Dim>* field : scalar_fields) {
        if (field && field->field_ID == field_id) {
            return field;
        }
    }
    return nullptr;
}

template<typename T, unsigned Dim>
Field<vec<T, Dim>, Dim>* TypedRegistry<T, Dim>::findVectorFieldByID(const std::string& field_id) {
    for (Field<vec<T, Dim>, Dim>* field : vector_fields) {
        if (field && field->field_ID == field_id) {
            return field;
        }
    }
    return nullptr;
}

template<typename T, unsigned Dim>
Field_b* TypedRegistry<T, Dim>::findFieldByID(const std::string& field_id) {
    // First try scalar fields
    auto* scalar_result = findScalarFieldByID(field_id);
    if (scalar_result) {
        return static_cast<Field_b*>(scalar_result);
    }
    
    // Then try vector fields
    auto* vector_result = findVectorFieldByID(field_id);
    if (vector_result) {
        return static_cast<Field_b*>(vector_result);
    }
    
    return nullptr;
}

template<typename T, unsigned Dim>
ParticleBase<T, Dim>* TypedRegistry<T, Dim>::findParticleByID(const std::string& bunch_id) {
    for (ParticleBase<T, Dim>* particle : particles) {
        if (particle && particle->bunch_ID == bunch_id) {
            return particle;
        }
    }
    return nullptr;
}

template<typename T, unsigned Dim>
void TypedRegistry<T, Dim>::listScalarFields() const {
    std::cout << "\n=== Typed Scalar Fields ===" << std::endl;
    if (scalar_fields.empty()) {
        std::cout << "No typed scalar fields registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < scalar_fields.size(); ++i) {
        if (scalar_fields[i]) {
            std::cout << "TS" << i << ": " << scalar_fields[i]->field_ID 
                      << " (data: " << scalar_fields[i]->data << ")" << std::endl;
        }
    }
}

template<typename T, unsigned Dim>
void TypedRegistry<T, Dim>::listVectorFields() const {
    std::cout << "\n=== Typed Vector Fields ===" << std::endl;
    if (vector_fields.empty()) {
        std::cout << "No typed vector fields registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < vector_fields.size(); ++i) {
        if (vector_fields[i]) {
            std::cout << "TV" << i << ": " << vector_fields[i]->field_ID << " (data: [";
            for (unsigned j = 0; j < Dim; ++j) {
                std::cout << vector_fields[i]->data[j];
                if (j < Dim - 1) std::cout << ", ";
            }
            std::cout << "])" << std::endl;
        }
    }
}

template<typename T, unsigned Dim>
void TypedRegistry<T, Dim>::listParticles() const {
    std::cout << "\n=== Typed Particles ===" << std::endl;
    if (particles.empty()) {
        std::cout << "No typed particles registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < particles.size(); ++i) {
        if (particles[i]) {
            std::cout << "TP" << i << ": " << particles[i]->bunch_ID 
                      << " (data: " << particles[i]->data << ")" << std::endl;
        }
    }
}

template<typename T, unsigned Dim>
void TypedRegistry<T, Dim>::listAllFields() const {
    listScalarFields();
    listVectorFields();
}

// Provide implementation for accessing configured registry without templates
inline TypedRegistryBase& VisBase::getConfiguredRegistry() {
    if (!type_config_set) {
        throw std::runtime_error("No registry has been configured yet. Create an object first.");
    }
    if (configured_registry) return *configured_registry;

    // Instantiate the correct typed registry instance lazily based on stored config
    if (registered_scalar_type_hash == typeid(double).hash_code()) {
        if (registered_dim == 1) configured_registry = &getTypedRegistry<double,1>();
        else if (registered_dim == 2) configured_registry = &getTypedRegistry<double,2>();
        else if (registered_dim == 3) configured_registry = &getTypedRegistry<double,3>();
    } else if (registered_scalar_type_hash == typeid(float).hash_code()) {
        if (registered_dim == 1) configured_registry = &getTypedRegistry<float,1>();
        else if (registered_dim == 2) configured_registry = &getTypedRegistry<float,2>();
        else if (registered_dim == 3) configured_registry = &getTypedRegistry<float,3>();
    } else if (registered_scalar_type_hash == typeid(int).hash_code()) {
        if (registered_dim == 1) configured_registry = &getTypedRegistry<int,1>();
        else if (registered_dim == 2) configured_registry = &getTypedRegistry<int,2>();
        else if (registered_dim == 3) configured_registry = &getTypedRegistry<int,3>();
    }

    if (!configured_registry) {
        throw std::runtime_error("Unsupported type combination for template-free access");
    }

    return *configured_registry;
}

// Template-free helpers delegating to the configured registry
inline size_t VisBase::getConfiguredScalarFieldCount() {
    return getConfiguredRegistry().getScalarFieldCount();
}

inline size_t VisBase::getConfiguredVectorFieldCount() {
    return getConfiguredRegistry().getVectorFieldCount();
}

inline size_t VisBase::getConfiguredParticleCount() {
    return getConfiguredRegistry().getParticleCount();
}

inline void VisBase::listConfiguredFields() {
    getConfiguredRegistry().listAllFields();
}

inline void VisBase::listConfiguredParticles() {
    getConfiguredRegistry().listParticles();
}

inline Field_b* VisBase::findConfiguredScalarFieldByID(const std::string& field_id) {
    return getConfiguredRegistry().findScalarFieldByIDBase(field_id);
}

inline Field_b* VisBase::findConfiguredVectorFieldByID(const std::string& field_id) {
    return getConfiguredRegistry().findVectorFieldByIDBase(field_id);
}

inline ParticleBase_b* VisBase::findConfiguredParticleByID(const std::string& bunch_id) {
    return getConfiguredRegistry().findParticleByIDBase(bunch_id);
}