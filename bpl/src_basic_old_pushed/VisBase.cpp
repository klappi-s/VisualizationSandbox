#include "VisBase.h"
#include "VisBase.hpp"
#include "field.h"
#include "particle.h"
#include "bpl.h"
#include <iostream>
#include <algorithm>

// Define static variables
std::vector<Field_b*> VisBase::sf_c;
std::vector<ParticleBase_b*> VisBase::pb_c;
bool VisBase::type_config_set = false;
size_t VisBase::registered_scalar_type_hash = 0;
unsigned VisBase::registered_dim = 0;

// Define bpl namespace variables and functions
namespace bpl {
    std::unique_ptr<VisBase> vb;
    
    void initializeVisualization() {
        bpl::vb = std::make_unique<VisBase>();
    }
}

// Find field by ID
Field_b* VisBase::findFieldByID(const std::string& field_id) {
    for (Field_b* field : sf_c) {
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

// List all fields
void VisBase::listAllFields() {
    std::cout << "\n=== Registered Fields ===" << std::endl;
    if (sf_c.empty()) {
        std::cout << "No fields registered." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (sf_c[i]) {
            std::cout << i << ": " << sf_c[i]->field_ID << std::endl;
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