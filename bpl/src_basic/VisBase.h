#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <type_traits>

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

// Type-erased registry interface
class TypedRegistryBase {
public:
    virtual ~TypedRegistryBase() = default;
    // Counts
    virtual size_t getScalarFieldCount() const = 0;
    virtual size_t getVectorFieldCount() const = 0;
    virtual size_t getParticleCount() const = 0;
    // Listing
    virtual void listAllFields() const = 0;
    virtual void listParticles() const = 0;
    // Finds (base-pointer returns to avoid template args)
    virtual Field_b* findScalarFieldByIDBase(const std::string& field_id) = 0;
    virtual Field_b* findVectorFieldByIDBase(const std::string& field_id) = 0;
    virtual ParticleBase_b* findParticleByIDBase(const std::string& bunch_id) = 0;
};

// Templated registry for specific type combinations (singleton pattern)
template<typename T, unsigned Dim>
class TypedRegistry : public TypedRegistryBase {
private:
    static TypedRegistry<T, Dim>* instance;
    
public:
    std::vector<Field<T, Dim>*> scalar_fields;
    std::vector<Field<vec<T, Dim>, Dim>*> vector_fields;  // VDim must equal Dim
    std::vector<ParticleBase<T, Dim>*> particles;
    
    // Singleton access
    static TypedRegistry<T, Dim>& getInstance() {
        if (!instance) {
            instance = new TypedRegistry<T, Dim>();
        }
        return *instance;
    }
    
    // Typed find functions that return proper types
    Field<T, Dim>* findScalarFieldByID(const std::string& field_id);
    Field<vec<T, Dim>, Dim>* findVectorFieldByID(const std::string& field_id);
    ParticleBase<T, Dim>* findParticleByID(const std::string& bunch_id);
    
    // General find that searches both scalar and vector fields
    Field_b* findFieldByID(const std::string& field_id);
    
    // Typed list helpers
    void listScalarFields() const;
    void listVectorFields() const;
    // listAllFields and listParticles also override the base interface
    void listAllFields() const override;
    void listParticles() const override;

    // Type-erased base overrides for counts and finds
    size_t getScalarFieldCount() const override { return scalar_fields.size(); }
    size_t getVectorFieldCount() const override { return vector_fields.size(); }
    size_t getParticleCount() const override { return particles.size(); }
    Field_b* findScalarFieldByIDBase(const std::string& field_id) override {
        return static_cast<Field_b*>(this->findScalarFieldByID(field_id));
    }
    Field_b* findVectorFieldByIDBase(const std::string& field_id) override {
        return static_cast<Field_b*>(this->findVectorFieldByID(field_id));
    }
    ParticleBase_b* findParticleByIDBase(const std::string& bunch_id) override {
        return static_cast<ParticleBase_b*>(this->findParticleByID(bunch_id));
    }
    
private:
    TypedRegistry() = default;
};

class VisBase{
    
    public:
    std::string vis_ID;
    
    // Legacy container access (for backward compatibility)
    virtual int get_size_pb(){return pb_c.size(); }
    virtual int get_size_sf(){return sf_c.size(); }
    virtual int get_size_vf(){return vf_c.size(); }
    
    // Legacy containers (kept for backward compatibility)
    static std::vector<Field_b*> sf_c;
    static std::vector<Field_b*> vf_c;
    static std::vector<ParticleBase_b*> pb_c;
    
    // Configuration validation
    static bool type_config_set;
    static size_t registered_scalar_type_hash;
    static unsigned registered_dim;
    
    // Store pointer to the configured registry instance for template-free access
    static TypedRegistryBase* configured_registry;
    
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
    
    // Helper functions to access typed registry
    template<typename T, unsigned Dim>
    static TypedRegistry<T, Dim>& getTypedRegistry() {
        return TypedRegistry<T, Dim>::getInstance();
    }

    // Access the configured registry without templates
    static TypedRegistryBase& getConfiguredRegistry();
    
    // Template-free access functions that use the configured registry
    static size_t getConfiguredScalarFieldCount();
    static size_t getConfiguredVectorFieldCount();
    static size_t getConfiguredParticleCount();
    static void listConfiguredFields();
    static void listConfiguredParticles();
    static Field_b* findConfiguredScalarFieldByID(const std::string& field_id);
    static Field_b* findConfiguredVectorFieldByID(const std::string& field_id);
    static ParticleBase_b* findConfiguredParticleByID(const std::string& bunch_id);
    
    // Field retrieval by ID (searches both scalar and vector fields)
    static Field_b* findFieldByID(const std::string& field_id);
    static Field_b* findScalarFieldByID(const std::string& field_id);
    static Field_b* findVectorFieldByID(const std::string& field_id);
    static ParticleBase_b* findParticleByID(const std::string& bunch_id);
    
    // Utility functions
    static void listAllFields();
    static void listAllScalarFields();
    static void listAllVectorFields();
    static void listAllParticles();
};










