#pragma once
#include <iostream>
#include "VisRegistry.h"

// Forward declaration to avoid circular include with bpl.h
namespace bpl { extern RegistryDynamic reg_g; }


/*  again problem with base class pointer virtual function auto and child member methods */
// class VisAdaptorBase{
// std::unique_ptr<RegistryBase> registry;


// ...existing code...

/*  if we can implement a hybird version we can implement a purely dynamic non templated version ... */


class VisAdaptorBase{
    using registry_t = RegistryDynamic;
    registry_t* registry = nullptr;
    bool owns_registry = false;
    // std::unique_ptr<RegistryBase> dynamic_registry;

public:

    // Constructor 
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

    // Initialize to use the global registry
    void init_global(){
        if (registry == &bpl::reg_g) return;
        if (owns_registry && registry){
            std::cerr << "[VisAdaptorBase] Warning: switching to global registry; disposing of local registry" << std::endl;
            delete registry;
        } else {
            std::cerr << "[VisAdaptorBase] Warning: switching to global registry" << std::endl;
        }
        registry = &bpl::reg_g;
        owns_registry = false;
    }

    // Late bind by compile-time ID
    template<fixed_string IdV, typename U>
    void add(U& obj) { registry->template Set<IdV>(obj); }

    // Retrieve and check
    template<fixed_string IdV>
    auto& get() const { return registry->template Get<IdV>(); }

    template<fixed_string IdV>
    bool contains() const { return registry->template Contains<IdV>(); }

    // Optional: unset by compile-time ID
    template<fixed_string IdV>
    bool remove() { return registry->template Unset<IdV>(); }

    // Backward-compat alias
    template<fixed_string IdV>
    auto& get_registry_entry() const { return registry->template Get<IdV>(); }

    // Runtime string API passthroughs
    template<typename T>
    void add_named(const std::string& name, T& object) { registry->add_named(name, object); }

    template<typename T>
    T* get_named(const std::string& name) const { return registry->get_named<T>(name); }

    bool contains_named(const std::string& name) const { return registry->contains_named(name); }

    bool remove_named(const std::string& name) { return registry->remove_named(name); }
};

// ...existing code...



