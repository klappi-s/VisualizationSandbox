#pragma once
#include "bpl.h"

/*  again problem with base class pointer virtual function auto and child member methods */
// class VisAdaptorBase{
// std::unique_ptr<RegistryBase> registry;



template <typename... Slots>
class VisAdaptorBase{
    using registry_t = RegistryTyped<Slots...>;
    std::shared_ptr<registry_t> registry;
    // std::unique_ptr<RegistryBase> dynamic_registry;

public:
    // Constructor from object references
    VisAdaptorBase(typename Slots::type&... objs) {
        registry = std::make_shared<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), &objs}...
            }
        );
    }

    // Optional: constructor from pointers
    VisAdaptorBase(typename Slots::type*... ptrs) {
        registry = std::make_shared<registry_t>(
            std::initializer_list<typename RegistryBase::Entry>{
                typename RegistryBase::Entry{std::string(Slots::Id.sv()), ptrs}...
            }
        );
    }

    // Construct by copying an existing registry object (creates a new shared instance)
    VisAdaptorBase(registry_t& r_init) {
        registry = std::make_shared<registry_t>(r_init);
    }

    // Share ownership of an existing registry
    VisAdaptorBase(std::shared_ptr<registry_t> reg)
        : registry(std::move(reg)) {}

    // Adopt from an existing unique_ptr (promote to shared_ptr)
    VisAdaptorBase(std::unique_ptr<registry_t>& reg)
        : registry(std::move(reg)) {}

    // Accessors returning a reference to the registry object (not the pointer)
    registry_t& get_registry() noexcept { return *registry; }
    const registry_t& get_registry() const noexcept { return *registry; }
    // If needed elsewhere, share the pointer too
    std::shared_ptr<registry_t> get_registry_ptr() const noexcept { return registry; }

    // Late bind by compile-time ID
    template<fixed_string IdV, typename U>
    void set(U& obj) {
        registry->template Set<IdV>(obj);
    }

    // Simplified, type-safe pointer setter (avoids private internals)
    template<fixed_string IdV, typename T>
    void set_ptr(T* ptr) {
        registry->template SetPtr<IdV>(ptr);
    }

    template<fixed_string IdV>
    bool contains() const {
        return registry->template Contains<IdV>();
    }

    // Retrieve and check
    template<fixed_string IdV>
    auto& get() const {
        return registry->template Get<IdV>();
    }
};

// Convenience Factory
template <fixed_string... Ids, typename... Ts>
auto MakeVisAdaptor(Ts&... objs)
    -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>
{
    return VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>(objs...);
}

// Overloads to build from existing registries
template <typename... Slots>
auto MakeVisAdaptor(std::shared_ptr<RegistryTyped<Slots...>> reg)
    -> VisAdaptorBase<Slots...>
{
    return VisAdaptorBase<Slots...>(std::move(reg));
}

template <typename... Slots>
auto MakeVisAdaptor(std::unique_ptr<RegistryTyped<Slots...>>& reg)
    -> VisAdaptorBase<Slots...>
{
    return VisAdaptorBase<Slots...>(reg);
}




