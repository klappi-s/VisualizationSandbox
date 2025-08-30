#pragma once
#include "bpl.h"



/*  again problem with base class pointer virtual function auto and child member methods */
// class VisAdaptorBase{
// std::unique_ptr<RegistryBase> registry;


// ...existing code...

template <typename... Slots>
class VisAdaptorBase{
    using registry_t = RegistryTyped<Slots...>;
    std::unique_ptr<registry_t> registry;
    // std::unique_ptr<RegistryBase> dynamic_registry;

public:
    // Constructor from object references
    VisAdaptorBase(typename Slots::type&... objs) {
        registry = std::make_unique<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), &objs}...
            }
        );
    }

    // Optional: constructor from pointers
    VisAdaptorBase(typename Slots::type*... ptrs) {
        registry = std::make_unique<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), ptrs}...
            }
        );
    }

    // Keep your existing helpers; fix 'Reg' -> 'registry_t'
    void MakeRegistry(typename Slots::type&... objs) {
        if (registry) std::cout << "WARNING: VisAdaptor already has registry; will be overwritten" << std::endl;
        registry = std::make_unique<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), &objs}...
            }
        );
    }

    void MakeRegistryPtrs(typename Slots::type*... ptrs) {
        if (registry) std::cout << "WARNING: VisAdaptor already has registry; will be overwritten" << std::endl;
        registry = std::make_unique<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), ptrs}...
            }
        );
    }

    template<fixed_string IdV>
    auto& get_registry_entry() const {
        return registry->template Get<IdV>();
    }

};
// ...existing code...



// Convenience Factory
template <fixed_string... Ids, typename... Ts>
auto MakeVisAdaptor(Ts&... objs)
    -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>
{
    return VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>(objs...);
}




