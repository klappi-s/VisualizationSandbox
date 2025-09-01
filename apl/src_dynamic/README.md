# APL: src_dynamic — Runtime registry (late-bound by compile-time IDs)

## Overview
Minimal dynamic registry that binds compile-time string IDs to runtime objects with type safety. Also exposes a flexible runtime string API. The thin, non-templated `VisAdaptorBase` owns the registry and forwards calls.

## Registry design
- Map-based storage: `std::unordered_map<std::string, void*>`.
- Compile-time Name→Type mapping via nested `NameToType<fixed_string>`; unknown names map to `void`.
- SFINAE guards ensure you can only `Set/Get/Contains/Unset` for IDs that were registered to a type.
- Optional runtime string API: `add_named/get_named/contains_named/remove_named`.

## Files
- `Vis_forward.h`, `field.h`, `particle.h`
- `VisRegistry.h` (RegistryDynamic), `VisBase.h` (facade)
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

## VisAdaptorBase API (facade over RegistryDynamic)
Ownership
- Holds `std::shared_ptr<RegistryDynamic>`
- Accessors:
  - `registry_t& get_registry()`
  - `const registry_t& get_registry() const`
  - `std::shared_ptr<registry_t> get_registry_ptr() const`

Compile-time ID API (type-checked via NameToType)
- `add<"ID">(obj)` — late-bind by reference (aliases registry `Set`)
- `get<"ID">() -> T&`
- `contains<"ID">() -> bool`
- `remove<"ID">() -> bool` — unset by ID

Runtime string API
- `add_named(name, obj)`
- `get_named<T>(name) -> T*` (nullptr if absent)
- `contains_named(name) -> bool`
- `remove_named(name) -> bool`

## Examples
Compile-time API (type-checked)
```cpp
VisAdaptorBase vis;
Field<double, 1> density;
vis.add<"density">(density);
std::cout << vis.get<"density">().data << "\n";
```

Runtime string API (flexible)
```cpp
VisAdaptorBase vis;
int score = 42;
vis.add_named("score", score);
if (auto* s = vis.get_named<int>("score")) {
    std::cout << *s << "\n";
}
```

Access the underlying registry
```cpp
auto& reg = vis.get_registry();              // reference
auto reg_ptr = vis.get_registry_ptr();       // shared_ptr if you need to share it

// direct compile-time calls on the registry
reg.template Set<"density">(density);
auto& d = reg.template Get<"density">();
```

## Provided types
- `vec<T, Dim>`: fixed-size vector over `std::array`
- `Field<T, Dim>`: simple field with `data`
- `ParticleBase<T, Dim>`: particle container

---

## Variants overview
- `src_static`: compile-time/static registry (typed). Shared registry ownership; maximal static safety.
- `src_fluent`: compile-time fluent/builder adaptor built over a typed registry; shared ownership, ergonomic `.add<...>()` chaining.
- `src_dynamic`: this module — dynamic, map-based late binding with compile-time ID→type mapping.
- `src_dynamic_auto`: dynamic registry with auto-registration hooks and helpers (see that folder for details).
