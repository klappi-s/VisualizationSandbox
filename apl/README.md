# VisualizationSandbox

This repository contains experiments around an IPPL-like layout to explore how to integrate visualization into a particle/field codebase. It’s also a refresher in modern C++, templates, and metaprogramming.

# APL (A Particle Layer)

APL provides a registry plus a thin adaptor that visualization code can use to discover and access fields and particle containers. The adaptor holds a smart pointer to a registry instance; other adaptor variants can build on the same base.

## Quick start (for all APL variants)
From any `apl/src_*` folder (e.g., `src_fluent`, `src_dynamic`, `src_dynamic_auto`):
- Build amain demo: `make amain` or `make`
- Run amain demo: `make run_amain`
- Build bdemo: `make bdemo`
- Run bdemo: `make run_bdemo`

Notes:
- The Registry does the main lifting; `VisAdaptorBase` is a facade that holds a smart pointer to a registry instance. Future adaptor variants can inherit from this base.
- Facade-style operators that manage the underlying Registry via the adaptor are optional.

## src_fluent — Compile-time typed registry with a fluent adaptor

A compile-time/static registry where types and dimensions are encoded in the registries type (via. template). Adds a fluent/builder-style adaptor so you can start empty and accumulate slots ergonomically in new updatet registry instances while preserving static guarantees.

### Registry (compile-time typed)
- Approach: Compile-time registry with ID→slot mapping resolved at compile time. Visitors and dispatch are resolved statically.
- Pros: Zero/low runtime overhead, maximal type safety.
- Cons: Adding new IDs/types requires recompilation; less flexible for plugins/runtime extension.

Template model
- Slots encode mappping for compile-time fixed_string IDs and their bound types: `Slot<Id, T>`.
- The registry is a templated type over its slots (see `VisRegistry.h`).

Construction
- Factories (helpers):
  - `MakeRegistry<Ids...>(objs...) -> std::unique_ptr<RegistryFluent<Slot<Ids, std::remove_reference_t<Ts>>...>>`
    - Binds each `Id` to the address of the corresponding `obj`.

Operations
- `Get<Id>() -> T&`
  - Returns a reference to the bound object. Throws if not bound.
- `Set<Id>(U& obj)` / `set(id<Id>, obj)`
  - Late-binds/rebinds by reference. Statically checks that `U` matches the slot type `T`.
- `Contains<Id>() -> bool` / `contains(id<Id>)`
  - True if the slot is currently bound (non-null pointer).
- `Unset<Id>()` / `unset(id<Id>)`
  - Removes a binding for the given Id.

Notes
- Internally stores raw pointers to user-owned objects.
- Strong compile-time checks: unknown Ids trigger `static_assert`.
- `field.h`, `particle.h` provide demo data types.

### VisAdaptorBase (fluent builder over the registry)
- Holds `std::shared_ptr` to the underlying registry type.
- Use `get_registry()` to call the full registry API directly: `get_registry().template Get<"ID">()`, `Set`, `Contains`, `Unset` (including tag-based overloads on the registry).
- Factories for adaptors:
  - `MakeVisAdaptor<Ids...>(Ts&... objs) -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>`
  - Or construct from an existing smart pointer to a registry with shared ownership: construct from `shared_ptr` or promote from a `unique_ptr`.
- Fluent accumulation:
  - Start empty with `VisAdaptorBase<>` and accumulate via `.add<"ID">(obj)` which returns a new adaptor type that includes the new slot. Note: `add` is rvalue-qualified; call as `std::move(vis).add<...>(obj)` or use helpers.
- Tag-based convenience:
  - Use `id<"density">` with the registry: `get_registry().Get(id<...>)`.


### TODO: adapt Fluent Accumulation option
You can start completely empty with `VisAdaptorBase<>` or with a factory helper. Use `MakeRegistry<Ids...>(objs...)` and later accumulate more slots via `add<"ID">(obj)` which returns a new adaptor type. (TODO: should actually be implemented for Reistry not adaptor!!!!).

#### Example usage
```cpp
// Create data
Field<vec<double,3>, 3> F1;
Field<vec<double,2>, 2> F2;
Field<vec<double,1>, 1> F3;
Field<double, 1> f1, f2;

// Create adaptor with new registry
auto vis = MakeVisAdaptor<"E","rho","phi","density">(F1, F2, F3, f1);
auto& reg = vis.get_registry();

// Access via registry reference
auto& E = reg.Get<"E">();

// Access another entry via the registry
auto& rho = reg.Get<"rho">();

// Late bind/overwrite via the registry
reg.Set<"density">(f2);

// Build adaptor from an existing registry
auto reg2 = MakeRegistry<"E","rho">(F1, F2);      // unique_ptr
auto vis2 = MakeVisAdaptor(reg2);                    // promoted to shared_ptr
auto& E2 = vis2.get_registry().template Get<"E">();

// Start empty and add slots fluently
VisAdaptorBase<> v0;
Field<double,1> rho_f;
auto v1 = std::move(v0).add<"density">(rho_f);
std::cout << v1.get_registry().template Get<"density">().data << "\n";

// Tag-based API on the registry
std::cout << vis.get_registry().Get(id<"E">).data << std::endl;
std::cout << reg.Get(id<"E">).data << std::endl;
```

## src_dynamic 
Dynamic, type-safe registry and adaptor

This module provides a minimal, type-safe registry that binds compile-time string IDs to concrete C++ types, plus a small facade (`VisAdaptorBase`) for easy use. It supports both compile-time (name-only) and runtime string APIs.

### Key Concepts
- Registry and adaptor are not templates.
- The registry maps “well-known” compile-time IDs (like "density") to types via a registration macro.
- At runtime you bind objects to those IDs and retrieve them with full static type safety.
- A separate runtime string API allows ad-hoc storage/retrieval with explicit types.

### Registration (ID → Type)
Declare the mapping once at namespace scope by specializing a mapping struct template defined inside the registry class:

```cpp
#define REGDYN_REGISTER_NAME_TYPE(name_lit, ...) \
    template<> struct RegistryDynamic::NameToType<fixed_string{name_lit}> { using type = __VA_ARGS__; }


REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("E", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho", Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi", Field<vec<double,1>, 1>);
```

This informs the registry that the compile-time ID corresponds to the given type.

### Registry API
Compile-time name-only API (requires registration):
- `Set<"ID">(obj)`
- `Get<"ID">() -> T&`
- `Contains<"ID">() -> bool`
- `Unset<"ID">() -> bool`
- Optionally: `reg.get(id<"name">)`

Runtime string API:
- `add_named(name, obj)`
- `get_named<T>(name) -> T*` (nullptr if absent or wrong type)
- `contains_named(name) -> bool`
- `remove_named(name) -> bool`

### VisAdaptorBase API (lifecycle-only facade)
- Holds a shared_ptr to a `RegistryDynamic` instance but does not forward registry operations.
- Use `get_registry()` to access the registry and call its API directly.
<!-- - You may keep a convenience `add<"ID">(obj)` on the adaptor. -->

### TODO:: possible Visitor patterns

Support for visitor patterns can be added for both runtime and compile-time APIs, with "Name/Id" knowledge only.


### Examples
Compile-time API (type-checked):
```cpp
VisAdaptorBase vis;
Field<double, 1> density;
vis.get_registry().Set<"density">(density);
std::cout << vis.get_registry().Get<"density">().data << "\n";
```

Pure Runtime string API (more flexible without prior type registration but slower and need type info at time of retrieval ...):
```cpp
VisAdaptorBase vis;
int score = 42;
vis.get_registry().add_named("score", score);
if (auto* s = vis.get_registry().get_named<int>("score")) {
    std::cout << *s << "\n";
}
```



## src_dynamic_auto (auto-registration with a single global instance)
Having a global registry and link a VisAdaptorBase Instance with `global_init()` call to this global object, would be possible with this dynamic approach, but we decided to avoid a global namespace instance for now.
- Approach: Dynamic registry with auto-registration. Field/particle construction hooks register instances automatically into the global registry (`VisBase`). Adds helpers like find-by-ID and print/visit utilities to reduce boilerplate.
- Pros: Minimal user code to participate in visualization; safer, more ergonomic dynamic approach.
- Cons: Care needed for construction order and lifetime; ...

---

## Variants overview
- `src_fluent`: compile-time typed registry with a fluent/builder adaptor. Ergonomic add<"ID"> chaining; static guarantees.
- `src_dynamic`: runtime map-based registry with compile-time ID→type mapping and runtime string API.
- `src_dynamic_auto`: dynamic registry with auto-registration hooks.

// ...existing code...

---

## Comparison: Fluent vs Dynamic (GPT-5)

| Approach | Pros | Cons | Use when |
|---|---|---|---|
| src_fluent (compile-time typed) | - Strong compile-time type safety<br>- Zero/low runtime overhead<br>- Static dispatch and IDE guidance<br>- Clear failure at compile time | - Recompilation to add IDs/types<br>- Less flexible for plugins/runtime extension<br>- In case for multiple or evolving registry > Template bloat<br>- More ceremony to evolve schemas | - Performance-critical code paths<br>- Stable, well-known schemas<br>- You want static guarantees and early errors |
| src_dynamic (runtime map + compile-time ID→type mapping) | - Single non-templated registry/adaptor type<br>- Bind/rebind at runtime<br>- Works well with modular/plugins<br>- Also offers a runtime string API | - Runtime lookups and errors (throws if missing)<br>- Slight overhead vs static approach<br>- Requires registration macros for ID→type mapping<br>- Lifetime management still on the user | - Extensible apps and plugins<br>- Prototyping and tooling integration<br>- You need to add/replace bindings at runtime |



It rests to mention that the fluent approach could be "extended" with some elements of the dynamic approach, or merged in some sorts (two in one registry).