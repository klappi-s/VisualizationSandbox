# APL: src_dynamic — Runtime registry (late-bound by compile-time IDs)

## Overview
Minimal dynamic registry that binds compile-time string IDs to runtime objects with type safety. Also exposes a flexible runtime string API. The thin `VisAdaptorBase` now only manages/holds a registry pointer; use `get_registry()` to operate on the registry.

## Registry design
- Map-based storage: `std::unordered_map<std::string, void*>`.
- Compile-time Name→Type mapping via nested `NameToType<fixed_string>`; unknown names map to `void`.
- SFINAE guards ensure you can only `Set/Get/Contains/Unset` for IDs that were registered to a type.
- Optional runtime string API: `add_named/get_named/contains_named/remove_named`.

## Files
- `Vis_forward.h`, `field.h`, `particle.h`
- `VisRegistry.h` (RegistryDynamic), `VisBase.h` (adaptor)
- `amain.cpp`, `bdemo.cpp`, `Makefile`

## Build & Run
- Build: `make`
- Run demos: `make run` (amain) or `make run_bdemo`

---

## Registering IDs (compile-time Name→Type)
Declare mappings at namespace scope using the macro:

```cpp
REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("E",       Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho",     Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi",     Field<vec<double,1>, 1>);
```

These inform the registry which type each compile-time ID refers to.

## VisAdaptorBase (lifecycle only)
Ownership/Access
- Holds `std::shared_ptr<RegistryDynamic>` which may be null by default.
- Constructors:
  - `VisAdaptorBase()` — default constructs with nullptr.
  - `explicit VisAdaptorBase(std::shared_ptr<RegistryDynamic>)` — inject an existing registry (must be non-null).
- Accessors:
  - `registry_t& get_registry()` / `const registry_t& get_registry() const` — dereference the current registry (must be non-null).
  - `std::shared_ptr<registry_t> get_registry_ptr() const` — retrieve the shared pointer.
- Reset:
  - `reset_registry(std::shared_ptr<RegistryDynamic>)` — replace held registry, warning if the previous one was non-empty.
  - `reset_registry(std::unique_ptr<RegistryDynamic>)` — promoted to shared and replaced with the same warning.

Note: No forwarding helpers like `add/get/contains` on the adaptor; operate directly on the registry via `get_registry()`.

## Examples
Operate directly on the registry
```cpp
VisAdaptorBase vis(std::make_shared<RegistryDynamic>());
auto& reg = vis.get_registry();

Field<double, 1> density;
reg.template Set<"density">(density);
std::cout << reg.template Get<"density">().data << "\n";

int score = 42;
reg.add_named("score", score);
if (auto* s = reg.get_named<int>("score")) {
    std::cout << *s << "\n";
}
```

## Provided types
- `vec<T, Dim>`: fixed-size vector over `std::array`
- `Field<T, Dim>`: simple field with `data`
- `ParticleBase<T, Dim>`: particle container

---

## Variants overview
- `src_fluent`: compile-time fluent/builder adaptor built over a typed registry; shared ownership, ergonomic chaining.
- `src_dynamic`: this module — dynamic, map-based late binding with compile-time ID→type mapping.
- `src_dynamic_auto`: dynamic registry with auto-registration hooks and helpers (see that folder for details).
