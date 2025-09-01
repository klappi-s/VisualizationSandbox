#pragma once
#include "bpl.h"

// Forward declaration to avoid circular include with bpl.h
namespace bpl { extern RegistryDynamic reg_g; }

// Forward declare fixed_string for NTTP usage in this header
template <std::size_t N>
struct fixed_string;

/*  again problem with base class pointer virtual function auto and child member methods */
// class VisAdaptorBase{
// std::unique_ptr<RegistryBase> registry;

/*  if we can implement a hybird version we can implement a purely dynamic non templated version ... */

class VisAdaptorBase{
    using registry_t = RegistryDynamic;
    registry_t* registry = nullptr;
    bool owns_registry = false;

public:
    // Default: own a local registry
    VisAdaptorBase(){
        registry = new registry_t();
        owns_registry = true;
    }

    ~VisAdaptorBase(){
        if (owns_registry && registry){
            delete registry;
            registry = nullptr;
        }
    }

    // Switch adaptor to use the global registry instance
    void init_global(){
        if (registry == &bpl::registry_g) return;
        if (owns_registry && registry){
            std::cerr << "[VisAdaptorBase] Warning: switching to global registry; disposing of local registry" << std::endl;
            delete registry;
        } else {
            std::cerr << "[VisAdaptorBase] Warning: switching to global registry" << std::endl;
        }
        registry = &bpl::registry_g;
        owns_registry = false;
    }

    // Access the underlying registry (operate directly on it)
    registry_t& get_registry() {
        if (!registry) throw std::logic_error("VisAdaptorBase: registry is null");
        return *registry;
    }
    const registry_t& get_registry() const {
        if (!registry) throw std::logic_error("VisAdaptorBase: registry is null");
        return *registry;
    }

    // Convenience: allow binding by compile-time ID via the adaptor (lifecycle-only)
    template<auto IdV, typename U>
    void add(U& obj) { get_registry().template Set<IdV>(obj); }

    // Note: Forwarding helpers for get/contains/remove and runtime string API
    // were removed. Use get_registry().Get/Contains/Unset and
    // get_registry().add_named/get_named/contains_named/remove_named instead.
};



