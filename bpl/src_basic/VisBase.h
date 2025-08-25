#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

// Forward declarations
class Field_b;
class ParticleBase_b;

template<typename T, unsigned Dim>
class Field;

template<typename T, unsigned Dim>
class ParticleBase;

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
    
    class VisBase{
public:
    std::string vis_ID;
    virtual int get_size_pb(){return pb_c.size(); }
    virtual int get_size_sf(){return sf_c.size(); }
    
    static std::vector<Field_b*> sf_c; 
    static std::vector<ParticleBase_b*> pb_c;
    
    // Configuration validation - must be set by user code
    static bool type_config_set;
    static size_t registered_scalar_type_hash;
    static unsigned registered_dim;
    
    // Type validation functions
    template<typename T, unsigned Dim>
    static void validateScalarFieldType();
    
    template<typename T, unsigned Dim, unsigned VDim>
    static void validateVectorFieldType();
    
    template<typename T, unsigned Dim>
    static void validateParticleType();
    
    // Initialize the type configuration (called once by first registration)
    template<typename T, unsigned Dim>
    static void initializeTypeConfig();
    
    // Simple field retrieval by ID
    static Field_b* findFieldByID(const std::string& field_id);
    static ParticleBase_b* findParticleByID(const std::string& bunch_id);
    
    // Utility functions
    static void listAllFields();
    static void listAllParticles();
};










