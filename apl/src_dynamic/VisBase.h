#pragma once
#include "bpl.h"

class VisAdaptorBase{
    using registry_t = RegistryDynamic;
    std::shared_ptr<registry_t> registry; // may be null until provided

public:
    // Default: no registry owned (nullptr)
    VisAdaptorBase() = default;

    // Construct with an existing shared registry instance
    explicit VisAdaptorBase(std::shared_ptr<registry_t> reg) {
        if (!reg) {
            throw std::invalid_argument("VisAdaptorBase: reg must not be null");
        }
        registry = std::move(reg);
    }

    // Construct with a unique_ptr registry and take ownership by promoting to shared_ptr
    explicit VisAdaptorBase(std::unique_ptr<registry_t>&& reg) {
        if (!reg) {
            throw std::invalid_argument("VisAdaptorBase: reg must not be null");
        }
        registry = std::shared_ptr<registry_t>(std::move(reg));
    }

    // Accessors to the underlying registry
    registry_t& get_registry() noexcept { return *registry; }
    const registry_t& get_registry() const noexcept { return *registry; }
    std::shared_ptr<registry_t> get_registry_ptr() const noexcept { return registry; }

    // Reset/replace the underlying registry instance.
    // Warn if the current registry contains bindings before replacement.
    void reset_registry(std::shared_ptr<registry_t> new_registry) {
        if (!new_registry) {
            throw std::invalid_argument("reset_registry: new_registry must not be null");
        }
        if (registry && !registry->empty()) {
            std::cerr << "[VisAdaptorBase] Warning: replacing a non-empty registry" << std::endl;
        }
        registry = std::move(new_registry);
    }

    // Overload: accept unique_ptr and take ownership by promoting to shared_ptr
    void reset_registry(std::unique_ptr<registry_t> new_registry) {
        if (!new_registry) {
            throw std::invalid_argument("reset_registry: new_registry must not be null");
        }
        if (registry && !registry->empty()) {
            std::cerr << "[VisAdaptorBase] Warning: replacing a non-empty registry" << std::endl;
        }
        registry = std::shared_ptr<registry_t>(std::move(new_registry));
    }

    // No facade mutators: operate via get_registry() directly.
};



