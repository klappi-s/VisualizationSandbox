#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include <iostream>
#include <string>
#include <typeinfo>

// Forward declarations
template<typename T, unsigned Dim, unsigned VDim>
class VisSubRegistry;

template<typename T, unsigned Dim>
class Field;

template<typename T, unsigned Dim>
class ParticleBase;

template<typename T, unsigned Dim>
struct vec;




// ===== OPTIMIZATION: Container Management Mixin =====
// DRY: Eliminates repeated container initialization code
template<typename ContainerType>
class ContainerManager {
protected:
    inline static std::unique_ptr<std::vector<ContainerType*>> container;
    
    static void ensure_container() noexcept {
        // if (!container) [[unlikely]] {
        if (!container) [[likely]] {
            container = std::make_unique<std::vector<ContainerType*>>();
        }
    }
    
public:
    static auto& get_container() noexcept {
        ensure_container();
        return container;
    }
};

// ===== OPTIMIZATION: Apply Function Utilities =====
// DRY: Eliminates repeated apply logic
template<typename ContainerType, unsigned DimValue, unsigned VDimValue = 1>
struct ApplyHelper {
    template<typename Func>
    static void apply_to_current(Func&& func) noexcept {
        auto& container = ContainerManager<ContainerType>::get_container();
        if (container) [[likely]] {
            for (auto* item : *container) {
                if (item) [[likely]] {
                    if constexpr (VDimValue == 1) {
                        // Check if function expects 2 parameters (Dim, VDim) or just 1 (Dim)
                        if constexpr (std::is_invocable_v<Func, ContainerType&, unsigned, unsigned>) {
                            func(*item, DimValue, VDimValue);
                        } else {
                            func(*item, DimValue);
                        }
                    } else {
                        func(*item, DimValue, VDimValue);
                    }
                }
            }
        }
    }
};





// Base case: Both Dim and VDim are 1
template<typename T>
class VisSubRegistry<T, 1, 1> : protected ContainerManager<Field<T,1>>,
                                protected ContainerManager<Field<vec<T,1>,1>>,
                                protected ContainerManager<ParticleBase<T,1>>
{
public:
    using vf_c_t = Field<vec<T,1>,1>;
    using pb_c_t = ParticleBase<T,1>;
    using sf_c_t = Field<T,1>;
    
    constexpr VisSubRegistry() noexcept = default;
    
    // Performance: Direct access to optimized containers
    static inline auto& sf_c = ContainerManager<sf_c_t>::get_container();
    static inline auto& vf_c = ContainerManager<vf_c_t>::get_container();
    static inline auto& pb_c = ContainerManager<pb_c_t>::get_container();

    // DRY: Optimized apply functions using helpers
    template<typename Func>
    void apply_to_current_sf_c(Func&& func) noexcept {
        ApplyHelper<sf_c_t, 1>::apply_to_current(std::forward<Func>(func));
    }

    template<typename Func>
    void apply_to_all_sf_c(Func&& func) noexcept {
        apply_to_current_sf_c(std::forward<Func>(func));
    }

    template<typename Func>
    void apply_to_current_vf_c(Func&& func) noexcept {
        ApplyHelper<vf_c_t, 1, 1>::apply_to_current(std::forward<Func>(func));
    }

    template<typename Func>
    void apply_to_all_vf_c(Func&& func) noexcept {
        apply_to_current_vf_c(std::forward<Func>(func));        
    }
};



// Horizontal axis: VDim > 1, Dim = 1
template<typename T, unsigned VDim>
class VisSubRegistry<T, 1, VDim> :
    public virtual VisSubRegistry<T, 1, VDim-1>,
    protected ContainerManager<Field<vec<T,VDim>,1>>
{
public:
    using vf_c_t = Field<vec<T,VDim>,1>;
    
    constexpr VisSubRegistry() noexcept = default;
    
    static inline auto& vf_c = ContainerManager<vf_c_t>::get_container();
    
    // DRY: Optimized apply functions
    template<typename Func>
    void apply_to_current_vf_c(Func&& func) noexcept {
        ApplyHelper<vf_c_t, 1, VDim>::apply_to_current(std::forward<Func>(func));
    }
    
    template<typename Func>
    void apply_to_all_vf_c(Func&& func) noexcept {
        apply_to_current_vf_c(std::forward<Func>(func));
        if constexpr (VDim > 1) {
            VisSubRegistry<T, 1, VDim-1>::apply_to_all_vf_c(std::forward<Func>(func));
        }
    }
};





// Vertical axis: Dim > 1, VDim = 1  
template<typename T, unsigned Dim>
class VisSubRegistry<T, Dim, 1> :
    public virtual VisSubRegistry<T, Dim-1, 1>,
    protected ContainerManager<Field<T,Dim>>,
    protected ContainerManager<Field<vec<T,1>,Dim>>,
    protected ContainerManager<ParticleBase<T,Dim>>
{
public:
    using pb_c_t = ParticleBase<T,Dim>;
    using vf_c_t = Field<vec<T,1>,Dim>;
    using sf_c_t = Field<T,Dim>;

    constexpr VisSubRegistry() noexcept = default;
    
    // Performance: Direct access to optimized containers
    static inline auto& sf_c = ContainerManager<sf_c_t>::get_container();
    static inline auto& vf_c = ContainerManager<vf_c_t>::get_container();
    static inline auto& pb_c = ContainerManager<pb_c_t>::get_container();

    // DRY: Optimized apply functions using helpers
    template<typename Func>
    void apply_to_current_sf_c(Func&& func) noexcept {
        ApplyHelper<sf_c_t, Dim>::apply_to_current(std::forward<Func>(func));
    }
    
    template<typename Func>
    void apply_to_current_pb_c(Func&& func) noexcept {
        ApplyHelper<pb_c_t, Dim>::apply_to_current(std::forward<Func>(func));
    }
    
    template<typename Func>
    void apply_to_current_vf_c(Func&& func) noexcept {
        ApplyHelper<vf_c_t, Dim, 1>::apply_to_current(std::forward<Func>(func));
    }
    
    // Recursively apply to all Dim levels
    template<typename Func>
    void apply_to_all_sf_c(Func&& func) noexcept {
        apply_to_current_sf_c(std::forward<Func>(func));
        
        if constexpr (Dim > 1) {
            VisSubRegistry<T, Dim-1, 1>::apply_to_all_sf_c(std::forward<Func>(func));
        }
    }
    
    template<typename Func>
    void apply_to_all_pb_c(Func&& func) noexcept {
        apply_to_current_pb_c(std::forward<Func>(func));
        
        if constexpr (Dim > 1) {
            VisSubRegistry<T, Dim-1, 1>::apply_to_all_pb_c(std::forward<Func>(func));
        }
    }

    template<typename Func>
    void apply_to_all_vf_c(Func&& func) noexcept {
        apply_to_current_vf_c(std::forward<Func>(func));
        
        if constexpr (Dim > 1) {
            VisSubRegistry<T, Dim-1, 1>::apply_to_all_vf_c(std::forward<Func>(func));
        }
    }
};





// General case: Both Dim > 1 and VDim > 1
template<typename T, unsigned Dim, unsigned VDim>
class VisSubRegistry : 
    public virtual VisSubRegistry<T, Dim-1, VDim>,
    public virtual VisSubRegistry<T, Dim, VDim-1>,
    protected ContainerManager<Field<vec<T,VDim>,Dim>>
{
public:
    using vf_c_t = Field<vec<T,VDim>,Dim>;
    
    constexpr VisSubRegistry() noexcept = default;
    
    static inline auto& vf_c = ContainerManager<vf_c_t>::get_container();
    
    // Access different Dim/VDim levels
    template<unsigned AccessDim, unsigned AccessVDim>
    static auto& get_vf_c() noexcept {
        return VisSubRegistry<T, AccessDim, AccessVDim>::vf_c;
    }
    
    // Apply function to all vf_c at this level
    template<typename Func>
    void apply_to_current_vf_c(Func&& func) noexcept {
        ApplyHelper<vf_c_t, Dim, VDim>::apply_to_current(std::forward<Func>(func));
    }
    
    // Recursively apply to all levels
    template<typename Func>
    void apply_to_all_vf_c(Func&& func) noexcept {
        apply_to_current_vf_c(std::forward<Func>(func));
        
        // Recurse on Dim axis
        if constexpr (Dim > 1) {
            VisSubRegistry<T, Dim-1, VDim>::apply_to_all_vf_c(std::forward<Func>(func));
        }
        
        // Recurse on VDim axis
        if constexpr (VDim > 1) {
            VisSubRegistry<T, Dim, VDim-1>::apply_to_all_vf_c(std::forward<Func>(func));
        }
    }
};

        


/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */
/* =============================================================================================== */









// Updated VisRegistry with all three apply functions
template<typename T, unsigned Dim>
class VisRegistry : 
    protected VisSubRegistry<T, Dim, 3>
{
public:
    constexpr VisRegistry() noexcept = default;

    // Getter functions - Performance: constexpr for compile-time optimization
    static constexpr auto& get_sf_c() noexcept { return VisSubRegistry<T, Dim, 1>::sf_c; }
    static constexpr auto& get_pb_c() noexcept { return VisSubRegistry<T, Dim, 1>::pb_c; }
    
    template<unsigned AccessDim, unsigned AccessVDim>
    static constexpr auto& get_vf_c() noexcept { 
        return VisSubRegistry<T, AccessDim, AccessVDim>::vf_c; 
    }
    



    
    // ===== FIELD RETRIEVAL BY ID FUNCTIONALITY =====
    
    // Find field by ID across all scalar field containers (returns void* for flexibility)
    template<unsigned SearchDim = Dim>
    static void* findScalarFieldByID_impl(const std::string& field_id) noexcept {
        auto& container = VisSubRegistry<T, SearchDim, 1>::sf_c;
        if (container) [[likely]] {
            for (auto* field : *container) {
                if (field && field->field_ID == field_id) [[likely]] {
                    return field;
                }
            }
        }
        
        // Recursively search lower dimensions
        if constexpr (SearchDim > 1) {
            return findScalarFieldByID_impl<SearchDim - 1>(field_id);
        }
        
        return nullptr;
    }
    
    // Type-safe wrapper for scalar field search
    template<unsigned SearchDim = Dim>
    static Field<T, SearchDim>* findScalarFieldByID(const std::string& field_id) noexcept {
        return static_cast<Field<T, SearchDim>*>(findScalarFieldByID_impl<SearchDim>(field_id));
    }




    
    // Find vector field by ID with specific VDim (returns void* for flexibility)
    template<unsigned SearchDim = Dim, unsigned SearchVDim = 3>
    static void* findVectorFieldByID_impl(const std::string& field_id) noexcept {
        auto& container = VisSubRegistry<T, SearchDim, SearchVDim>::vf_c;
        if (container) [[likely]] {
            for (auto* field : *container) {
                if (field && field->field_ID == field_id) [[likely]] {
                    return field;
                }
            }
        }
        
        // Recursively search other dimensions
        if constexpr (SearchDim > 1) {
            void* result = findVectorFieldByID_impl<SearchDim - 1, SearchVDim>(field_id);
            if (result) return result;
        }
        
        if constexpr (SearchVDim > 1) {
            return findVectorFieldByID_impl<SearchDim, SearchVDim - 1>(field_id);
        }
        
        return nullptr;
    }
    
    // Type-safe wrapper for vector field search
    template<unsigned SearchDim = Dim, unsigned SearchVDim = 3>
    static Field<vec<T, SearchVDim>, SearchDim>* findVectorFieldByID(const std::string& field_id) noexcept {
        return static_cast<Field<vec<T, SearchVDim>, SearchDim>*>(
            findVectorFieldByID_impl<SearchDim, SearchVDim>(field_id));
    }
    



    
    // Generic field finder that searches both scalar and vector fields
    static void* findFieldByID(const std::string& field_id) noexcept {
        // Try scalar fields first
        if (void* scalar_field = findScalarFieldByID_impl(field_id)) {
            return scalar_field;
        }
        
        // Try vector fields
        if (void* vector_field = findVectorFieldByID_impl(field_id)) {
            return vector_field;
        }
        
        return nullptr;
    }
    



    // ===== PARTICLE RETRIEVAL BY ID FUNCTIONALITY =====
    
    // Find particle by ID across all particle containers (returns void* for flexibility)
    template<unsigned SearchDim = Dim>
    static void* findParticleByID_impl(const std::string& particle_id) noexcept {
        auto& container = VisSubRegistry<T, SearchDim, 1>::pb_c;
        if (container) [[likely]] {
            for (auto* particle : *container) {
                if (particle && particle->bunch_ID == particle_id) [[likely]] {
                    return particle;
                }
            }
        }
        
        // Recursively search lower dimensions
        if constexpr (SearchDim > 1) {
            return findParticleByID_impl<SearchDim - 1>(particle_id);
        }
        
        return nullptr;
    }
    
    // Type-safe wrapper for particle search
    template<unsigned SearchDim = Dim>
    static ParticleBase<T, SearchDim>* findParticleByID(const std::string& particle_id) noexcept {
        return static_cast<ParticleBase<T, SearchDim>*>(findParticleByID_impl<SearchDim>(particle_id));
    }
    






    // ===== UTILITY FUNCTIONS =====
    
    // Get data from field by ID (returns std::optional for safety)
    template<typename DataType = T>
    static std::optional<DataType> getFieldData(const std::string& field_id) noexcept {
        if (auto* field = findScalarFieldByID(field_id)) {
            if constexpr (std::is_same_v<DataType, T>) {
                return field->data;
            }
        }
        return std::nullopt;
    }
    
    // Get vector field data by ID
    template<unsigned VDim, typename DataType = vec<T, VDim>>
    static std::optional<DataType> getVectorFieldData(const std::string& field_id) noexcept {
        if (auto* field = findVectorFieldByID<Dim, VDim>(field_id)) {
            return field->data;
        }
        return std::nullopt;
    }
    
    // Get particle data by ID
    template<typename DataType = std::vector<std::array<T, Dim>>>
    static std::optional<DataType> getParticleData(const std::string& particle_id) noexcept {
        if (auto* particle = findParticleByID(particle_id)) {
            return particle->values;
        }
        return std::nullopt;
    }
    






    // Print field information by ID
    static void printFieldInfo(const std::string& field_id) noexcept {
        std::cout << "=== Field Information ===" << std::endl;
        std::cout << "Searching for field ID: '" << field_id << "'" << std::endl;
        
        // Search scalar fields
        if (void* scalar_field_ptr = findScalarFieldByID_impl(field_id)) {
            auto* scalar_field = static_cast<Field<T, Dim>*>(scalar_field_ptr);
            std::cout << "Found scalar field!" << std::endl;
            std::cout << "Type: Field<" << typeid(T).name() << ", " << Dim << ">" << std::endl;
            std::cout << "Data: " << scalar_field->data << std::endl;
            return;
        }
        
        // Search vector fields  
        if (void* vector_field_ptr = findVectorFieldByID_impl(field_id)) {
            std::cout << "Found vector field!" << std::endl;
            std::cout << "Type: Field<vec<" << typeid(T).name() << ", ?>, ?>" << std::endl;
            // Note: Could enhance to print actual dimensions and data
            return;
        }
        
        std::cout << "Field not found!" << std::endl;
    }
    
    // Print particle information by ID
    static void printParticleInfo(const std::string& particle_id) noexcept {
        std::cout << "=== Particle Information ===" << std::endl;
        std::cout << "Searching for particle ID: '" << particle_id << "'" << std::endl;
        
        if (void* particle_ptr = findParticleByID_impl(particle_id)) {
            auto* particle = static_cast<ParticleBase<T, Dim>*>(particle_ptr);
            std::cout << "Found particle!" << std::endl;
            std::cout << "Type: ParticleBase<" << typeid(T).name() << ", " << Dim << ">" << std::endl;
            std::cout << "Number of values: " << particle->values.size() << std::endl;
            
            // Print first few values if available
            if (!particle->values.empty()) {
                std::cout << "Sample values: ";
                for (size_t i = 0; i < std::min(size_t(3), particle->values.size()); ++i) {
                    std::cout << "[";
                    for (unsigned j = 0; j < Dim; ++j) {
                        std::cout << particle->values[i][j];
                        if (j < Dim - 1) std::cout << ", ";
                    }
                    std::cout << "] ";
                }
                std::cout << std::endl;
            }
            return;
        }
        
        std::cout << "Particle not found!" << std::endl;
    }
    
    // Print the entire state of the registry in a nice formatted way
    void print_state() const noexcept;







    // ===== APPLY FUNCTIONS TO SINGLE FIELDS BY ID =====

    // Apply function to all vector fields across the entire 2D grid
    template<typename Func>
    void apply_to_all_vector_fields(Func&& func) noexcept(noexcept(func)) {
        VisSubRegistry<T, Dim, 3>::apply_to_all_vf_c(std::forward<Func>(func));
    }
    
    // Apply function to all scalar fields across all Dim levels
    template<typename Func>
    void apply_to_all_scalar_fields(Func&& func) noexcept(noexcept(func)) {
        VisSubRegistry<T, Dim, 1>::apply_to_all_sf_c(std::forward<Func>(func));
    }
    
    // Apply function to all particle bunches across all Dim levels
    template<typename Func>
    void apply_to_all_particle_bunches(Func&& func) noexcept(noexcept(func)) {
        VisSubRegistry<T, Dim, 1>::apply_to_all_pb_c(std::forward<Func>(func));
    }
    




    // ===== APPLY FUNCTIONS TO SINGLE FIELDS BY ID =====
    
    // Apply function to a single scalar field identified by ID
    template<typename Func>
    bool apply_to_scalar_field_by_id(const std::string& field_id, Func&& func) noexcept(noexcept(func)) {
        // Helper lambda to search and apply at a specific dimension
        auto search_and_apply = [&]<unsigned SearchDim>() -> bool {
            if constexpr (SearchDim <= Dim) {
                auto& container = VisSubRegistry<T, SearchDim, 1>::sf_c;
                if (container) {
                    for (auto* field : *container) {
                        if (field && field->field_ID == field_id) {
                            // Apply function with proper signature detection
                            if constexpr (std::is_invocable_v<Func, Field<T, SearchDim>&, unsigned>) {
                                func(*static_cast<Field<T, SearchDim>*>(field), SearchDim);
                            } else {
                                func(*static_cast<Field<T, SearchDim>*>(field));
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        
        // Search through all dimensions
        if constexpr (Dim >= 3) {
            if (search_and_apply.template operator()<3>()) return true;
        }
        if constexpr (Dim >= 2) {
            if (search_and_apply.template operator()<2>()) return true;
        }
        if constexpr (Dim >= 1) {
            if (search_and_apply.template operator()<1>()) return true;
        }
        
        return false; // Field not found
    }
    
    // Apply function to a single vector field identified by ID
    template<typename Func>
    bool apply_to_vector_field_by_id(const std::string& field_id, Func&& func) noexcept(noexcept(func)) {
        // Helper lambda to search and apply at specific (Dim, VDim) combination
        auto search_and_apply = [&]<unsigned SearchDim, unsigned SearchVDim>() -> bool {
            if constexpr (SearchDim <= Dim && SearchVDim <= 3) {
                auto& container = VisSubRegistry<T, SearchDim, SearchVDim>::vf_c;
                if (container) {
                    for (auto* field : *container) {
                        if (field && field->field_ID == field_id) {
                            // Apply function with proper signature detection
                            if constexpr (std::is_invocable_v<Func, Field<vec<T, SearchVDim>, SearchDim>&, unsigned, unsigned>) {
                                func(*static_cast<Field<vec<T, SearchVDim>, SearchDim>*>(field), SearchDim, SearchVDim);
                            } else {
                                func(*static_cast<Field<vec<T, SearchVDim>, SearchDim>*>(field));
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        
        // Search through all (Dim, VDim) combinations
        if constexpr (Dim >= 3) {
            if (search_and_apply.template operator()<3, 3>()) return true;
            if (search_and_apply.template operator()<3, 2>()) return true;
            if (search_and_apply.template operator()<3, 1>()) return true;
        }
        if constexpr (Dim >= 2) {
            if (search_and_apply.template operator()<2, 2>()) return true;
            if (search_and_apply.template operator()<2, 3>()) return true;
            if (search_and_apply.template operator()<2, 1>()) return true;
        }
        if constexpr (Dim >= 1) {
            if (search_and_apply.template operator()<1, 1>()) return true;
            if (search_and_apply.template operator()<1, 3>()) return true;
            if (search_and_apply.template operator()<1, 2>()) return true;
        }
        
        return false; // Field not found
    }
    
    // Apply function to a single particle bunch identified by ID
    template<typename Func>
    bool apply_to_particle_bunch_by_id(const std::string& bunch_id, Func&& func) noexcept(noexcept(func)) {
        // Helper lambda to search and apply at a specific dimension
        auto search_and_apply = [&]<unsigned SearchDim>() -> bool {
            if constexpr (SearchDim <= Dim) {
                auto& container = VisSubRegistry<T, SearchDim, 1>::pb_c;
                if (container) {
                    for (auto* particle : *container) {
                        if (particle && particle->bunch_ID == bunch_id) {
                            // Apply function with proper signature detection
                            if constexpr (std::is_invocable_v<Func, ParticleBase<T, SearchDim>&, unsigned>) {
                                func(*static_cast<ParticleBase<T, SearchDim>*>(particle), SearchDim);
                            } else {
                                func(*static_cast<ParticleBase<T, SearchDim>*>(particle));
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        
        // Search through all dimensions
        if constexpr (Dim >= 1) {
            if (search_and_apply.template operator()<1>()) return true;
        }
        if constexpr (Dim >= 2) {
            if (search_and_apply.template operator()<2>()) return true;
        }
        if constexpr (Dim >= 3) {
            if (search_and_apply.template operator()<3>()) return true;
        }
        
        return false; // Particle bunch not found
    }
    
    // Apply function to any field (scalar or vector) identified by ID
    // This method automatically detects whether the field is scalar or vector
    // and calls the appropriate specialized method
    template<typename Func>
    bool apply_to_field_by_id(const std::string& field_id, Func&& func) noexcept(noexcept(func)) {
        // First try to find it as a scalar field
        if (apply_to_scalar_field_by_id(field_id, std::forward<Func>(func))) {
            return true;
        }
        
        // If not found as scalar, try to find it as a vector field
        if (apply_to_vector_field_by_id(field_id, std::forward<Func>(func))) {
            return true;
        }
        
        // Field not found in either scalar or vector containers
        return false;
    }
    




    

};

