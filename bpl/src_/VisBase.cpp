#include "VisBase.h"
#include "VisBase.hpp"
#include "field.h"
#include "bpl.h"
#include <iostream>
#include <typeindex>

// Define bpl namespace variables and functions
namespace bpl {
    std::unique_ptr<VisBase> vb;
    
    void initializeVisualization() {
        bpl::vb = std::make_unique<VisBase>();
    }
}

// Internal helper for registering type handlers
void VisBase::registerTypeHandlers(size_t type_hash, DataExtractor extractor, FieldCreator creator) {
    data_extractors[type_hash] = extractor;
    field_creators[type_hash] = creator;
}

// Find field by ID
Field_b* VisBase::findFieldByID(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it != field_id_map.end() && it->second < sf_c.size()) {
        return sf_c[it->second];
    }
    return nullptr;
}

// Get field data using registered extractors
std::any VisBase::getFieldData(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        std::cout << "Field '" << field_id << "' not found!" << std::endl;
        return std::any{};
    }
    
    size_t index = it->second;
    const TypeInfo& info = sf_type_info[index];
    auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
    Field_b* field = sf_c[index];
    
    // Use registered data extractor for this type
    auto extractor_it = data_extractors.find(type_hash);
    if (extractor_it != data_extractors.end()) {
        return extractor_it->second(field);
    }
    
    std::cout << "No data extractor registered for type hash: " << type_hash << std::endl;
    return std::any{};
}

// Recreate field using registered creators
std::unique_ptr<Field_b> VisBase::recreateField(size_t index) {
    if (index >= sf_c.size() || index >= sf_type_info.size()) {
        return nullptr;
    }
    
    const TypeInfo& info = sf_type_info[index];
    auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
    Field_b* original = sf_c[index];
    std::any data = getFieldData(original->field_ID);
    
    // Use registered field creator for this type
    auto creator_it = field_creators.find(type_hash);
    if (creator_it != field_creators.end()) {
        return creator_it->second(data, original->field_ID + "_copy");
    }
    
    std::cout << "No field creator registered for type hash: " << type_hash << std::endl;
    return nullptr;
}

// Print field information
void VisBase::printFieldInfo(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end()) {
        std::cout << "Field '" << field_id << "' not found!" << std::endl;
        return;
    }
    
    size_t index = it->second;
    const TypeInfo& info = sf_type_info[index];
    auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
    std::cout << "=== Field Information ===" << std::endl;
    std::cout << "ID: " << field_id << std::endl;
    std::cout << "Index: " << index << std::endl;
    std::cout << "Dimension: " << dim << std::endl;
    std::cout << "Is Vector: " << (is_vector ? "Yes" : "No") << std::endl;
    if (is_vector) {
        std::cout << "Vector Dimension: " << vdim << std::endl;
    }
    
    // Try to get and display data
    std::any data = getFieldData(field_id);
    if (data.has_value()) {
        std::cout << "Data successfully retrieved" << std::endl;
    } else {
        std::cout << "Failed to retrieve data" << std::endl;
    }
}

// List all fields
void VisBase::listAllFields() {
    std::cout << "=== All Registered Fields ===" << std::endl;
    std::cout << "Total fields: " << sf_c.size() << std::endl;
    
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (i < sf_type_info.size()) {
            const TypeInfo& info = sf_type_info[i];
            auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
            
            std::cout << "[" << i << "] ID: " << sf_c[i]->field_ID 
                      << " | Dim: " << dim;
            if (is_vector) {
                std::cout << " | VDim: " << vdim << " (vector)";
            } else {
                std::cout << " (scalar)";
            }
            std::cout << std::endl;
        } else {
            std::cout << "[" << i << "] ID: " << sf_c[i]->field_ID 
                      << " | No type info" << std::endl;
        }
    }
}