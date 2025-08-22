#pragma once

#include <memory>
#include <vector>
#include <utility>

// Forward declaration
template<typename T, unsigned Dim, unsigned VDim>
class VisSubRegistry;




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
    
    // Print the entire state of the registry in a nice formatted way
    void print_state() const noexcept;
};

