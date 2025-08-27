// Enhanced VisBase.cpp with improved user experience
#include "VisBase.h"
#include "field.h"
#include "particle.h"

#include <iostream>
#include <typeindex>


// Find field by ID
Field_b* VisBase::findFieldByID(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it != field_id_map.end() && it->second < sf_c.size()) {
        return sf_c[it->second];
    }
    return nullptr;
}

// Find particle by ID (new implementation)
ParticleBase_b* VisBase::findParticleByID(const std::string& particle_id) {
    for (auto* particle : pb_c) {
        if (particle && particle->bunch_ID == particle_id) {
            return particle;
        }
    }
    return nullptr;
}


// Print field information (existing)
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
    // std::any data = getFieldData(field_id);
    // if (data.has_value()) {
    //     std::cout << "Data successfully retrieved" << std::endl;
    // } else {
    //     std::cout << "Failed to retrieve data" << std::endl;
    // }
}


// Print List of all exisitng Fields
void VisBase::PrintListFields() {
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


template<typename FT, unsigned Dim>
void VisBase::registerFieldType(Field_b* field_ptr) {

    constexpr bool is_vector = is_vector_type_v<FT>; 
    constexpr unsigned vdim = vector_dimension_v<FT>;
    
    // For vector types, we want the scalar type hash; for scalar types, it's the same as type hash
    size_t scalar_hash;
    if constexpr (is_vector) {
        scalar_hash = typeid(scalar_type_t<FT>).hash_code();
    } else {
        scalar_hash = typeid(FT).hash_code();
    }
    
    TypeInfo type_info = std::make_tuple(
        typeid(FT).hash_code(),
        Dim,
        vdim,
        is_vector,
        scalar_hash
    );
    
    sf_type_info.push_back(type_info);



    // Add field to global registry and create ID mapping
    field_id_map[field_ptr->field_ID] = sf_c.size() - 1;

}





// getFieldRef_byType: Returns reference to Field object with exact type match
template<typename FT, unsigned Dim>
std::optional<std::reference_wrapper<Field<FT, Dim>>> VisBase::getFieldRef_byType(const std::string& field_id) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        return std::nullopt;
    }
    
    size_t index = it->second;
    const TypeInfo& info = sf_type_info[index];
    auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
    // Check if the requested type and dimension match
    if (type_hash != typeid(FT).hash_code() || dim != Dim) {
        std::cout << "The requested Field_ID and the specified Type Info do not seem to match" << std::endl;
        return std::nullopt;
    }
    
    Field_b* field_ptr = sf_c[index];
    if (auto* typed_field = dynamic_cast<Field<FT, Dim>*>(field_ptr)) {
        return std::ref(*typed_field);
    }
    
    return std::nullopt;
}

// Virtual functions
int VisBase::get_size_pb() {
    return pb_c.size();
}

int VisBase::get_size_sf() {
    return sf_c.size();
}

// Static inline function
bool VisBase::hasField(const std::string& field_id) {
    return field_id_map.find(field_id) != field_id_map.end();
}

// Template function definitions
template<typename T, unsigned Dim>
std::vector<std::string> VisBase::getListFieldsOfType_ID() {
    std::vector<std::string> result;
    size_t target_hash = typeid(T).hash_code();
    
    for (const auto& [field_id, index] : field_id_map) {
        if (index < sf_type_info.size()) {
            const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[index];
            if (type_hash == target_hash && dim == Dim) {
                result.push_back(field_id);
            }
        }
    }
    return result;
}

// Apply to all functions
template<typename Func>
void VisBase::apply_to_all_scalar_fields(Func&& func) {
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (i < sf_type_info.size()) {
            const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
            if (!is_vector) {
                // This is a scalar field, apply the function
                func(sf_c[i], dim);
            }
        }
    }
}

template<typename Func>
void VisBase::apply_to_all_vector_fields(Func&& func) {
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (i < sf_type_info.size()) {
            const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
            if (is_vector) {
                // This is a vector field, apply the function
                func(sf_c[i], dim, vdim);
            }
        }
    }
}

template<typename Func>
void VisBase::apply_to_all_fields(Func&& func) {
    for (size_t i = 0; i < sf_c.size(); ++i) {
        if (i < sf_type_info.size()) {
            const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
            if (is_vector) {
                func(sf_c[i], dim, vdim, true);  // true indicates vector field
            } else {
                func(sf_c[i], dim, 0, false);   // false indicates scalar field, vdim=0
            }
        }
    }
}

// Visitor pattern functions
template<typename Visitor, typename ScalarType>
auto VisBase::visit_field_of_scalar_type(Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor) {
    if (!is_vector) {
        // Scalar field dispatch
        switch (dim) {
            case 3: return visitor(*static_cast<Field<ScalarType, 3>*>(field_ptr));
            case 1: return visitor(*static_cast<Field<ScalarType, 1>*>(field_ptr));
            case 2: return visitor(*static_cast<Field<ScalarType, 2>*>(field_ptr));
            default: throw std::runtime_error("Unsupported field dimension");
        }
    } else {
        // Vector field dispatch
        switch (vdim) {
            case 2:
                switch (dim) {
                    case 1: return visitor(*static_cast<Field<vec<ScalarType, 2>, 1>*>(field_ptr));
                    case 2: return visitor(*static_cast<Field<vec<ScalarType, 2>, 2>*>(field_ptr));
                    case 3: return visitor(*static_cast<Field<vec<ScalarType, 2>, 3>*>(field_ptr));
                    default: throw std::runtime_error("Unsupported field dimension");
                }
            case 3:
                switch (dim) {
                    case 1: return visitor(*static_cast<Field<vec<ScalarType, 3>, 1>*>(field_ptr));
                    case 2: return visitor(*static_cast<Field<vec<ScalarType, 3>, 2>*>(field_ptr));
                    case 3: return visitor(*static_cast<Field<vec<ScalarType, 3>, 3>*>(field_ptr));
                    default: throw std::runtime_error("Unsupported field dimension");
                }
            case 4:
                switch (dim) {
                    case 1: return visitor(*static_cast<Field<vec<ScalarType, 4>, 1>*>(field_ptr));
                    case 2: return visitor(*static_cast<Field<vec<ScalarType, 4>, 2>*>(field_ptr));
                    case 3: return visitor(*static_cast<Field<vec<ScalarType, 4>, 3>*>(field_ptr));
                    default: throw std::runtime_error("Unsupported field dimension");
                }
            default: throw std::runtime_error("Unsupported vector dimension");
        }
    }
}

// Base case: Ran out of types in the tuple, so the type is unsupported.
template<typename Visitor>
auto VisBase::visit_field_dispatcher(size_t scalar_hash, const std::string& field_id, Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor, std::tuple<>) {
    // This is only reached if no type in the tuple matched.
    throw std::runtime_error("Unsupported scalar type for field: " + field_id);
}

// Recursive step: Check the first type in the tuple.
template<typename Visitor, typename Head, typename... Tail>
auto VisBase::visit_field_dispatcher(size_t scalar_hash, const std::string& field_id, Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor, std::tuple<Head, Tail...>) {
    // Check if the runtime hash matches the hash of the current type (Head)
    if (scalar_hash == typeid(Head).hash_code()) {
        // Match found! Call the actual implementation function and return.
        return visit_field_of_scalar_type<Visitor, Head>(field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor));
    } else {
        // No match, recurse with the rest of the types (Tail...)
        return visit_field_dispatcher(scalar_hash, field_id, field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor), std::tuple<Tail...>{});
    }
}

template<typename Visitor, typename... ScalarTypes>
auto VisBase::visit_field_impl(const std::string& field_id, bool expect_vector, Visitor&& visitor, std::tuple<ScalarTypes...>) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        throw std::runtime_error("Field not found: " + field_id);
    }
    
    size_t index = it->second;
    const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[index];
    
    if (expect_vector && !is_vector) {
        throw std::runtime_error("Field '" + field_id + "' is a scalar field, not a vector field");
    }
    if (!expect_vector && is_vector) {
        throw std::runtime_error("Field '" + field_id + "' is a vector field, not a scalar field");
    }
    
    Field_b* field_ptr = sf_c[index];

    // Kick off the compile-time dispatch process
    return visit_field_dispatcher(scalar_hash, field_id, field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor), std::tuple<ScalarTypes...>{});
}

template<typename Visitor>
auto VisBase::visit_field_impl_2(const std::string& field_id, bool expect_vector, Visitor&& visitor) {
    auto it = field_id_map.find(field_id);

    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        throw std::runtime_error("Field not found: " + field_id);
    }

    size_t index = it->second;
    const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[index];

    if (expect_vector && !is_vector) {
        throw std::runtime_error("Field '" + field_id + "' is a scalar field, not a vector field");
    }
    if (!expect_vector && is_vector) {
        throw std::runtime_error("Field '" + field_id + "' is a vector field, not a scalar field");
    }

    Field_b* field_ptr = sf_c[index];
    // Dispatch based on scalar type
    if (scalar_hash == typeid(double).hash_code()) {
        return visit_field_of_scalar_type<Visitor, double>(field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor));
    }
    else if (scalar_hash == typeid(float).hash_code()) {
        return visit_field_of_scalar_type<Visitor, float>(field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor));
    }
    else if (scalar_hash == typeid(int).hash_code()) {
        return visit_field_of_scalar_type<Visitor, int>(field_ptr, dim, vdim, is_vector, std::forward<Visitor>(visitor));
    }
    throw std::runtime_error("Unsupported scalar type for field: " + field_id);
}

// Visit specific scalar field by ID
template<typename Visitor>
auto VisBase::visit_scalar_field(const std::string& field_id, Visitor&& visitor) {
    return visit_field_impl(field_id, false, std::forward<Visitor>(visitor), supported_types{});
}

// Visit specific vector field by ID
template<typename Visitor>
auto VisBase::visit_vector_field(const std::string& field_id, Visitor&& visitor) {
    return visit_field_impl(field_id, true, std::forward<Visitor>(visitor), supported_types{});
}

// Generic visit (auto-detects field type)
template<typename Visitor>
auto VisBase::visit_field(const std::string& field_id, Visitor&& visitor) {
    auto it = field_id_map.find(field_id);
    if (it == field_id_map.end() || it->second >= sf_c.size()) {
        throw std::runtime_error("Field not found: " + field_id);
    }
    
    size_t index = it->second;
    const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[index];
    
    return visit_field_impl(field_id, is_vector, std::forward<Visitor>(visitor), supported_types{});
    // return visit_field_impl_2(field_id, is_vector, std::forward<Visitor>(visitor));
}
