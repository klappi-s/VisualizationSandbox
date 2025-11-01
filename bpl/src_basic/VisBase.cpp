#include "VisBase.h"
#include "VisBase.hpp"
#include "field.h"
#include "particle.h"
#include "bpl.h"
#include <iostream>
#include <algorithm>

// Define static variables
std::vector<Field_b*> VisBase::sf_c;
std::vector<Field_b*> VisBase::vf_c;  // Vector fields container
std::vector<ParticleBase_b*> VisBase::pb_c;
bool VisBase::type_config_set = false;
size_t VisBase::registered_scalar_type_hash = 0;
unsigned VisBase::registered_dim = 0;
TypedRegistryBase* VisBase::configured_registry = nullptr;

// Define bpl namespace variables and functions
namespace bpl {
    std::unique_ptr<VisBase> vb;
    
    void initializeVisualization() {
        bpl::vb = std::make_unique<VisBase>();
    }
}

// Find field by ID (searches both scalar and vector fields)
Field_b* VisBase::findFieldByID(const std::string& field_id) {
    // Search scalar fields first
    Field_b* found = findScalarFieldByID(field_id);
    if (found) return found;
    
    // Search vector fields
    return findVectorFieldByID(field_id);
}

// Find scalar field by ID
Field_b* VisBase::findScalarFieldByID(const std::string& field_id) {
    for (Field_b* field : sf_c) {
        if (field && field->field_ID == field_id) {
            return field;
        }
    }
    return nullptr;
}

// Find vector field by ID
Field_b* VisBase::findVectorFieldByID(const std::string& field_id) {
    for (Field_b* field : vf_c) {
        if (field && field->field_ID == field_id) {
            return field;
        }
    }
    return nullptr;
}

// Find particle by ID
ParticleBase_b* VisBase::findParticleByID(const std::string& bunch_id) {
    for (ParticleBase_b* particle : pb_c) {
        if (particle && particle->bunch_ID == bunch_id) {
            return particle;
        }
    }
    return nullptr;
}

// List all fields (both scalar and vector)
void VisBase::listAllFields() {
    std::cout << "\n=== All Registered Fields ===" << std::endl;
    
    int total_fields = sf_c.size() + vf_c.size();
    if (total_fields == 0) {
        std::cout << "No fields registered." << std::endl;
        return;
    }
    
    listAllScalarFields();
    listAllVectorFields();
}

// List scalar fields
void VisBase::listAllScalarFields() {
    std::cout << "\n=== Registered Scalar Fields ===" << std::endl;
    if (sf_c.empty()) {
        std::cout << "No scalar fields registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (sf_c[i]) {
            std::cout << "S" << i << ": " << sf_c[i]->field_ID << std::endl;
        }
    }
}

// List vector fields
void VisBase::listAllVectorFields() {
    std::cout << "\n=== Registered Vector Fields ===" << std::endl;
    if (vf_c.empty()) {
        std::cout << "No vector fields registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < vf_c.size(); ++i) {
        if (vf_c[i]) {
            std::cout << "V" << i << ": " << vf_c[i]->field_ID << std::endl;
        }
    }
}

// List all particles
void VisBase::listAllParticles() {
    std::cout << "\n=== Registered Particles ===" << std::endl;
    if (pb_c.empty()) {
        std::cout << "No particles registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < pb_c.size(); ++i) {
        if (pb_c[i]) {
            std::cout << i << ": " << pb_c[i]->bunch_ID << std::endl;
        }
    }
}

// Note: Template-free configured accessors are implemented inline in VisBase.hpp