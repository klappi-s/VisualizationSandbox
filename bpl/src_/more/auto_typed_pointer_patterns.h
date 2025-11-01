#pragma once
#include <iostream>
#include <string>
#include <any>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

#include "VIS_forward.h"

/*
 * AUTO TYPED POINTER PATTERNS
 * 
 * These patterns allow getting typed pointers without knowing template parameters
 * and without using visitor patterns.
 */

// Forward declarations
class Field_b;
template<typename T, unsigned Dim> class Field;

// =====================================================================
// APPROACH 1: TYPE REGISTRY WITH FACTORY FUNCTIONS
// =====================================================================

// Type-erased wrapper that can convert to any Field type
class TypedFieldHandle {
private:
    Field_b* field_ptr;
    size_t type_hash;
    unsigned dim;
    unsigned vdim;
    bool is_vector;
    size_t scalar_type_hash;
    
    // Function pointer to cast to specific type
    std::function<void*(Field_b*)> cast_function;
    
public:
    TypedFieldHandle(Field_b* ptr, size_t th, unsigned d, unsigned vd, bool iv, size_t sth,
                     std::function<void*(Field_b*)> cast_fn)
        : field_ptr(ptr), type_hash(th), dim(d), vdim(vd), is_vector(iv), 
          scalar_type_hash(sth), cast_function(cast_fn) {}
    
    // Auto-cast to any compatible type
    template<typename T>
    T* as() const {
        if (!field_ptr || !cast_function) return nullptr;
        return static_cast<T*>(cast_function(field_ptr));
    }
    
    // Get raw typed pointer (void*) - can be cast later
    void* getRawTyped() const {
        if (!field_ptr || !cast_function) return nullptr;
        return cast_function(field_ptr);
    }
    
    // Type information getters
    bool isVector() const { return is_vector; }
    unsigned getDim() const { return dim; }
    unsigned getVDim() const { return vdim; }
    size_t getTypeHash() const { return type_hash; }
    size_t getScalarTypeHash() const { return scalar_type_hash; }
    
    // Base field access
    Field_b* getBase() const { return field_ptr; }
    
    // Check if valid
    bool isValid() const { return field_ptr != nullptr && cast_function != nullptr; }
};

// =====================================================================
// APPROACH 2: SMART AUTO-DETECTING WRAPPER
// =====================================================================

class AutoFieldPtr {
private:
    Field_b* field_ptr;
    mutable void* cached_typed_ptr = nullptr;
    mutable bool cache_valid = false;
    
    // Runtime type detection and casting
    void* detectAndCast() const;
    
public:
    explicit AutoFieldPtr(Field_b* ptr) : field_ptr(ptr) {}
    
    // Automatic type detection and return
    template<typename ExpectedType>
    ExpectedType* get() const {
        if (!field_ptr) return nullptr;
        
        if (!cache_valid) {
            cached_typed_ptr = detectAndCast();
            cache_valid = true;
        }
        
        return static_cast<ExpectedType*>(cached_typed_ptr);
    }
    
    // Get without specifying type (returns void* that can be cast)
    void* getAuto() const {
        if (!cache_valid) {
            cached_typed_ptr = detectAndCast();
            cache_valid = true;
        }
        return cached_typed_ptr;
    }
    
    // Type information
    std::string getTypeString() const;
    bool isScalar() const;
    bool isVector() const;
    
    // Base access
    Field_b* getBase() const { return field_ptr; }
    bool isValid() const { return field_ptr != nullptr; }
};

// =====================================================================
// APPROACH 3: MACRO-BASED AUTO CASTING
// =====================================================================

// Macro that tries all known types and returns the first match
#define AUTO_CAST_FIELD(field_ptr, result_var) \
    auto result_var = [&]() -> void* { \
        if (auto* f = dynamic_cast<Field<double, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<double, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<double, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<double, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<float, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<float, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<float, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<float, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<int, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<int, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<int, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<int, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 1>, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 1>, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 1>, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 1>, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 2>, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 2>, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 2>, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 2>, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 3>, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 3>, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 3>, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 3>, 4>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 4>, 1>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 4>, 2>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 4>, 3>*>(field_ptr)) return f; \
        if (auto* f = dynamic_cast<Field<vec<double, 4>, 4>*>(field_ptr)) return f; \
        return nullptr; \
    }()

// =====================================================================
// APPROACH 4: TEMPLATE METAPROGRAMMING AUTO-DETECTION
// =====================================================================

template<typename... Types>
struct TypeList {};

// All supported field types
using SupportedScalarTypes = TypeList<double, float, int>;
using SupportedDimensions = std::integer_sequence<unsigned, 1, 2, 3, 4>;
using SupportedVectorDimensions = std::integer_sequence<unsigned, 1, 2, 3, 4>;

template<typename Visitor>
class FieldTypeDispatcher {
public:
    template<typename ScalarType, unsigned Dim>
    static auto tryScalarType(Field_b* field_ptr, Visitor&& visitor) {
        if (auto* typed = dynamic_cast<Field<ScalarType, Dim>*>(field_ptr)) {
            return visitor(typed);
        }
        return decltype(visitor(typed)){};
    }
    
    template<typename ScalarType, unsigned VDim, unsigned Dim>
    static auto tryVectorType(Field_b* field_ptr, Visitor&& visitor) {
        if (auto* typed = dynamic_cast<Field<vec<ScalarType, VDim>, Dim>*>(field_ptr)) {
            return visitor(typed);
        }
        return decltype(visitor(typed)){};
    }
    
    // Automatically try all combinations
    template<typename ScalarType>
    static auto dispatchScalar(Field_b* field_ptr, Visitor&& visitor) {
        if (auto result = tryScalarType<ScalarType, 1>(field_ptr, visitor)) return result;
        if (auto result = tryScalarType<ScalarType, 2>(field_ptr, visitor)) return result;
        if (auto result = tryScalarType<ScalarType, 3>(field_ptr, visitor)) return result;
        if (auto result = tryScalarType<ScalarType, 4>(field_ptr, visitor)) return result;
        return decltype(visitor(static_cast<Field<ScalarType, 1>*>(nullptr))){};
    }
    
    static auto dispatch(Field_b* field_ptr, Visitor&& visitor) {
        // Try scalar types first
        if (auto result = dispatchScalar<double>(field_ptr, visitor)) return result;
        if (auto result = dispatchScalar<float>(field_ptr, visitor)) return result;
        if (auto result = dispatchScalar<int>(field_ptr, visitor)) return result;
        
        // Try vector types
        // ... similar pattern for vectors ...
        
        return decltype(visitor(static_cast<Field<double, 1>*>(nullptr))){};
    }
};

// =====================================================================
// VisBase INTEGRATION - AUTO FIELD RETRIEVAL
// =====================================================================

class VisBase {
public:
    // Method 1: Get typed handle (no template parameters needed)
    static TypedFieldHandle getAutoFieldHandle(const std::string& field_id);
    
    // Method 2: Get auto-detecting wrapper
    static AutoFieldPtr getAutoField(const std::string& field_id) {
        Field_b* field = findFieldByID(field_id);
        return AutoFieldPtr(field);
    }
    
    // Method 3: Macro-based auto cast
    static void* getAutoFieldPtr(const std::string& field_id) {
        Field_b* field = findFieldByID(field_id);
        if (!field) return nullptr;
        
        AUTO_CAST_FIELD(field, result);
        return result;
    }
    
    // Method 4: Template metaprogramming dispatch
    template<typename Visitor>
    static auto visitAutoField(const std::string& field_id, Visitor&& visitor) {
        Field_b* field = findFieldByID(field_id);
        if (!field) return decltype(visitor(static_cast<Field<double, 1>*>(nullptr))){};
        
        return FieldTypeDispatcher<Visitor>::dispatch(field, std::forward<Visitor>(visitor));
    }

private:
    static std::vector<Field_b*> sf_c;
    static std::unordered_map<std::string, size_t> field_id_map;
    static Field_b* findFieldByID(const std::string& field_id);
    
    // Registry for cast functions
    static std::unordered_map<size_t, std::function<void*(Field_b*)>> cast_registry;
};

/*
 * USAGE EXAMPLES:
 * 
 * // Method 1: TypedFieldHandle
 * auto handle = VisBase::getAutoFieldHandle("my_field");
 * if (handle.isValid()) {
 *     auto* typed = handle.as<Field<double, 3>>();  // No need to know beforehand!
 *     if (typed) {
 *         typed->data[0] = 42.0;
 *     }
 * }
 * 
 * // Method 2: AutoFieldPtr
 * auto auto_field = VisBase::getAutoField("my_field");
 * if (auto_field.isValid()) {
 *     // This automatically detects and returns the correct type
 *     void* typed_ptr = auto_field.getAuto();
 *     // Cast to whatever type you expect, or use type info to determine
 * }
 * 
 * // Method 3: Macro-based
 * void* typed_ptr = VisBase::getAutoFieldPtr("my_field");
 * if (typed_ptr) {
 *     // typed_ptr is already the correct Field<T, Dim>* type
 *     // You can cast it or use type info to determine what it is
 * }
 * 
 * // Method 4: Template metaprogramming
 * auto result = VisBase::visitAutoField("my_field", [](auto* field) {
 *     if (field) {
 *         std::cout << "Field type automatically detected!" << std::endl;
 *         field->data[0] *= 2.0;  // Works with any field type
 *         return true;
 *     }
 *     return false;
 * });
 */
