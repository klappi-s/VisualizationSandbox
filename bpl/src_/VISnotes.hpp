















// Add this to the VisBase class

// Type-erased field pointer wrapper
// class TypedFieldPtr {
// private:
//     void* ptr;
//     size_t type_hash;
//     unsigned dim;
//     unsigned vdim;
//     bool is_vector;
    
// public:
//     TypedFieldPtr(void* p, size_t th, unsigned d, unsigned vd, bool iv) 
//         : ptr(p), type_hash(th), dim(d), vdim(vd), is_vector(iv) {}
    
//     // Template method to get properly typed pointer
//     template<typename T, unsigned Dim>
//     Field<T, Dim>* as() const {
//         // Verify type and dimension match
//         if (typeid(T).hash_code() == type_hash && Dim == dim) {
//             return static_cast<Field<T, Dim>*>(ptr);
//         }
//         return nullptr;
//     }
    
//     // Auto-detect and return properly typed pointer
//     template<typename T, unsigned Dim>
//     auto get() const -> Field<T, Dim>* {
//         return as<T, Dim>();
//     }
    
//     // Helper methods to check type properties
//     bool isVector() const { return is_vector; }
//     unsigned getDim() const { return dim; }
//     unsigned getVDim() const { return vdim; }
//     size_t getTypeHash() const { return type_hash; }
    
//     // Check if conversion is valid
//     template<typename T, unsigned Dim>
//     bool canConvertTo() const {
//         return typeid(T).hash_code() == type_hash && Dim == dim;
//     }
    
//     // Raw pointer access (use with caution)
//     void* getRawPtr() const { return ptr; }
    
//     // Base class access
//     Field_b* getBase() const { return static_cast<Field_b*>(ptr); }
// };

// // The main function that returns a typed field pointer
// static std::optional<TypedFieldPtr> get_field_ptr(const std::string& field_id);



// Add this implementation

// std::optional<VisBase::TypedFieldPtr> VisBase::get_field_ptr(const std::string& field_id) {
//     auto it = field_id_map.find(field_id);
//     if (it == field_id_map.end() || it->second >= sf_c.size()) {
//         return std::nullopt;
//     }
    
//     size_t index = it->second;
//     const TypeInfo& info = sf_type_info[index];
//     auto [type_hash, dim, vdim, is_vector, scalar_type_hash] = info;
    
//     Field_b* field_ptr = sf_c[index];
    
//     return TypedFieldPtr(static_cast<void*>(field_ptr), type_hash, dim, vdim, is_vector);
// }
























//     // Compile-time dimension lists
//     template<unsigned... Dims>
//     struct dim_list {};
    
//     template<unsigned... VDims>  
//     struct vdim_list {};
    
//     using supported_dims = dim_list<1, 2, 3>;
//     using supported_vdims = vdim_list<1, 2, 3>;
//     using supported_types = std::tuple<double, float, int>;
// // Support for double, float, int - easily extensible
    
//     // Recursive scalar field visitor
//     template<typename Visitor, typename ScalarType, unsigned... Dims>
//     static auto visit_scalar_field_recursive(Field_b* field_ptr, unsigned target_dim, Visitor&& visitor, dim_list<Dims...>) {
//         auto try_dim = [&]<unsigned D>() -> std::optional<decltype(visitor(*static_cast<Field<ScalarType, D>*>(field_ptr)))> {
//             if (target_dim == D) {
//                 return visitor(*static_cast<Field<ScalarType, D>*>(field_ptr));
//             }
//             return std::nullopt;
//         };
        
//         std::optional result = std::nullopt;
//         ((result = try_dim.template operator()<Dims>(), result.has_value()) || ...);
        
//         if (result.has_value()) {
//             return *result;
//         }
//         throw std::runtime_error("Unsupported scalar field dimension: " + std::to_string(target_dim));
//     }
    
//     // Recursive vector field visitor  
//     template<typename Visitor, typename ScalarType, unsigned VDim, unsigned... Dims>
//     static auto visit_vector_field_for_vdim(Field_b* field_ptr, unsigned target_dim, Visitor&& visitor, dim_list<Dims...>) {
//         auto try_dim = [&]<unsigned D>() -> std::optional<decltype(visitor(*static_cast<Field<vec<ScalarType, VDim>, D>*>(field_ptr)))> {
//             if (target_dim == D) {
//                 return visitor(*static_cast<Field<vec<ScalarType, VDim>, D>*>(field_ptr));
//             }
//             return std::nullopt;
//         };
        
//         std::optional result = std::nullopt;
//         ((result = try_dim.template operator()<Dims>(), result.has_value()) || ...);
        
//         if (result.has_value()) {
//             return *result;
//         }
//         throw std::runtime_error("Unsupported vector field dimension: " + std::to_string(target_dim));
//     }
    
//     template<typename Visitor, typename ScalarType, unsigned... VDims>
//     static auto visit_vector_field_recursive(Field_b* field_ptr, unsigned target_dim, unsigned target_vdim, Visitor&& visitor, vdim_list<VDims...>) {
//         auto try_vdim = [&]<unsigned VD>() -> std::optional<decltype(visit_vector_field_for_vdim<Visitor, ScalarType, VD>(field_ptr, target_dim, std::forward<Visitor>(visitor), supported_dims{}))> {
//             if (target_vdim == VD) {
//                 return visit_vector_field_for_vdim<Visitor, ScalarType, VD>(field_ptr, target_dim, std::forward<Visitor>(visitor), supported_dims{});
//             }
//             return std::nullopt;
//         };
        
//         std::optional result = std::nullopt;
//         ((result = try_vdim.template operator()<VDims>(), result.has_value()) || ...);
        
//         if (result.has_value()) {
//             return *result;
//         }
//         throw std::runtime_error("Unsupported vector dimension: " + std::to_string(target_vdim));
//     }
    
//     // Unified visitor dispatch with type list
//     template<typename Visitor, typename... ScalarTypes>
//     static auto visit_field_with_types(Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, size_t scalar_hash, Visitor&& visitor, std::tuple<ScalarTypes...>) {
//         auto try_type = [&]<typename T>() -> std::optional<decltype(visitor(*static_cast<Field<T, 1>*>(field_ptr)))> {
//             if (scalar_hash == typeid(T).hash_code()) {
//                 if (!is_vector) {
//                     return visit_scalar_field_recursive<Visitor, T>(field_ptr, dim, std::forward<Visitor>(visitor), supported_dims{});
//                 } else {
//                     return visit_vector_field_recursive<Visitor, T>(field_ptr, dim, vdim, std::forward<Visitor>(visitor), supported_vdims{});
//                 }
//             }
//             return std::nullopt;
//         };
        
//         std::optional result = std::nullopt;
//         ((result = try_type.template operator()<ScalarTypes>(), result.has_value()) || ...);
        
//         if (result.has_value()) {
//             return *result;
//         }
//         throw std::runtime_error("Unsupported scalar type");
//     }

// public:
//     // Ultra-DRY master visitor function
//     template<typename Visitor>
//     static auto visit_field_impl(const std::string& field_id, std::optional<bool> expect_vector, Visitor&& visitor) {
//         auto it = field_id_map.find(field_id);
//         if (it == field_id_map.end() || it->second >= sf_c.size()) {
//             throw std::runtime_error("Field not found: " + field_id);
//         }
        
//         size_t index = it->second;
//         const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[index];
        
//         if (expect_vector.has_value()) {
//             if (*expect_vector && !is_vector) {
//                 throw std::runtime_error("Field '" + field_id + "' is a scalar field, not a vector field");
//             }
//             if (!*expect_vector && is_vector) {
//                 throw std::runtime_error("Field '" + field_id + "' is a vector field, not a scalar field");
//             }
//         }
        
//         Field_b* field_ptr = sf_c[index];
        
//         return visit_field_with_types(field_ptr, dim, vdim, is_vector, scalar_hash, std::forward<Visitor>(visitor), supported_types{});
//     }
    
//     // Public interface methods
//     template<typename Visitor>
//     static auto visit_scalar_field(const std::string& field_id, Visitor&& visitor) {
//         return visit_field_impl(field_id, false, std::forward<Visitor>(visitor));
//     }
    
//     template<typename Visitor>
//     static auto visit_vector_field(const std::string& field_id, Visitor&& visitor) {
//         return visit_field_impl(field_id, true, std::forward<Visitor>(visitor));
//     }
    
//     template<typename Visitor>
//     static auto visit_field(const std::string& field_id, Visitor&& visitor) {
//         return visit_field_impl(field_id, std::nullopt, std::forward<Visitor>(visitor));
//     }

// // ...existing code...






































/*  for learning */
/* sfinae */

//     private:
//     // Tag types for dispatch
//     struct scalar_field_tag {};
//     struct vector_field_tag {};
//     struct all_fields_tag {};
    
    
//     template<typename Func, typename FieldTag>
//     static void apply_to_fields_impl(Func&& func, FieldTag tag) {
//         for (size_t i = 0; i < sf_c.size(); ++i) {
//             if (i < sf_type_info.size()) {
//                 const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
                
//                 if constexpr (std::is_same_v<FieldTag, scalar_field_tag>) {
//                     if (!is_vector) {
//                         func(sf_c[i], dim);
//                     }
//                 } else if constexpr (std::is_same_v<FieldTag, vector_field_tag>) {
//                     if (is_vector) {
//                         func(sf_c[i], dim, vdim);
//                     }
//                 } else if constexpr (std::is_same_v<FieldTag, all_fields_tag>) {
//                     // Handle both scalar and vector fields
//                     if (is_vector) {
//                         func(sf_c[i], dim, vdim, true);
//                     } else {
//                         func(sf_c[i], dim, 0, false);
//                     }
//                 }
//             }
//         }
//     }

// public:
//     // Apply function to all fields using the DRY infrastructure
//     template<typename Func>
//     static void apply_to_all_fields(Func&& func) {
//         apply_to_fields_impl(std::forward<Func>(func), all_fields_tag{});
//     }
//     // Apply function to all scalar fields
//     template<typename Func>
//     static void apply_to_all_scalar_fields(Func&& func) {
//         apply_to_fields_impl(std::forward<Func>(func), scalar_field_tag{});
//     }
    
//     // Apply function to all vector fields
//     template<typename Func>
//     static void apply_to_all_vector_fields(Func&& func) {
//         apply_to_fields_impl(std::forward<Func>(func), vector_field_tag{});
//     }




/* for learning predicate types */

// ...existing code...

// private:
//     // Predicate types for field filtering
//     struct IsScalarField {
//         bool operator()(const TypeInfo& info) const {
//             return !std::get<3>(info); // !is_vector
//         }
//     };
    
//     struct IsVectorField {
//         bool operator()(const TypeInfo& info) const {
//             return std::get<3>(info); // is_vector
//         }
//     };
    
//     // Generic field application with compile-time predicate
//     template<typename Func, typename Predicate>
//     static void apply_to_fields_matching(Func&& func, Predicate predicate = {}) {
//         for (size_t i = 0; i < sf_c.size(); ++i) {
//             if (i < sf_type_info.size() && predicate(sf_type_info[i])) {
//                 if constexpr (std::is_same_v<Predicate, IsScalarField>) {
//                     const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
//                     func(sf_c[i], dim);
//                 } else if constexpr (std::is_same_v<Predicate, IsVectorField>) {
//                     const auto& [type_hash, dim, vdim, is_vector, scalar_hash] = sf_type_info[i];
//                     func(sf_c[i], dim, vdim);
//                 }
//             }
//         }
//     }

// public:
//     // Apply function to all scalar fields
//     template<typename Func>
//     static void apply_to_all_scalar_fields(Func&& func) {
//         apply_to_fields_matching(std::forward<Func>(func), IsScalarField{});
//     }
    
//     // Apply function to all vector fields
//     template<typename Func>
//     static void apply_to_all_vector_fields(Func&& func) {
//         apply_to_fields_matching(std::forward<Func>(func), IsVectorField{});
//     }

// ...existing code...