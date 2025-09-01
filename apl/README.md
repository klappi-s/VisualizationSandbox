# VisualizationSandbox

This repository contains experiments around an IPPL-like layout to explore how to integrate visualization into a particle/field codebase. It’s also a refresher in modern C++, templates, and metaprogramming.

# APL (A Particle Layer)

APL provides a registry plus a thin adaptor that visualization code can use to discover and access fields and particle containers. The adaptor holds a smart pointer to a registry instance; other adaptor variants can build on the same base.

## Quick start (for all APL variants)
From any `apl/src_*` folder:
- Build amain demo: `make amain` or `make`
- Run amain demo: `make run_amain`
- Build bdemo: `make bdemo`
- Run bdemo: `make run_bdemo`

Notes:
- The Registry does the main lifting; `VisAdaptorBase` is a facade that holds a smart pointer to a registry instance. Future adaptor variants can inherit from this base.
- Facade-style operators that manage the underlying Registry via the adaptor are optional.

## src_static
### RegistryTyped 

- Approach: Compile-time/static registry. Types and dimensions are encoded in the type system (templates). Visitors and dispatch are resolved at compile time.
- Pros: Zero/low runtime overhead, maximal type safety.
- Cons: Adding new types requires recompilation; less flexible for plugins/runtime extension.

Template: `template <typename... Slots> class RegistryTyped` where each slot is `Slot<Id, T>`.

Construction
- Factories (helpers):
  - `MakeRegistry<Ids...>(objs...) -> std::unique_ptr<RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>>`
    - Binds each `Id` to the address of the corresponding `obj`.
    
Operations
- `Get<Id>() -> T&`
  - Returns a reference to the bound object. Throws if not bound.
- `Set<Id>(U& obj)`
  - Late-binds/rebinds by reference. Statically checks that `U` matches the slot type `T`.
- `Contains<Id>() -> bool`
  - True if the slot is currently bound (non-null pointer).
- `Unset<Id>()`
  - Removes a binding for the given Id.

Notes
- Internally stores raw pointers to user-owned objects.
- Strong compile-time checks: unknown Ids trigger `static_assert`.
- `field.h`, `particle.h`: demo data types.

### VisAdaptorBase
- `get`/`set`/`contains`/`unset` mirror the underlying registry operations.
- `get_registry()` returns a reference to the underlying registry. Throws if not bound.
- You can create a visualization adaptor directly with a factory helper (similar to the registry factory). It creates the underlying registry:
  `MakeVisAdaptor<Ids...>(Ts&... objs) -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>` 
- Or construct from an existing smart pointer to a registry with shared ownership: `VisAdaptorBase` holds `std::shared_ptr<RegistryTyped<...>>`, can be constructed from an existing `shared_ptr` or promoted from a `unique_ptr`.

#### Adaptor and Registry Example Usage
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
auto E = reg.Get<"E">();

// Or access through the adaptor
auto rho = vis.get<"rho">();

// Late bind/overwrite
vis.set<"density">(f2);

// Build adaptor from an existing registry
auto reg2 = MakeRegistry<"E","rho">(F1, F2);      // unique_ptr
auto vis2 = MakeVisAdaptor(reg2);                 // promoted to shared_ptr
auto E2 = vis2.get<"E">();
```

## src_fluent
A fluent, compile-time adaptor/registry variant. Mirrors `src_static` layout and APIs. You can start completely empty with `VisAdaptorBase<>` or with a factory helper. Use `MakeRegistry<Ids...>(objs...)` and later accumulate more slots via `add<"ID">(obj)` which returns a new adaptor type.

Usage examples
- Start empty and add slots fluently:
```cpp
VisAdaptorBase<> vis;
Field<double,1> rho;
auto vis2 = std::move(vis).add<"density">(rho);
std::cout << vis2.get<"density">().data << "\n";
```
- Macro helper: `VIS_REBIND` enables accumulating under effectively the same identifier in the following scope (not ideal):
```cpp
Field<double, 1> f1, f2;
VisAdaptorBase<> v0;

// Normally you need to change the adaptor name...
auto v1 = std::move(v0).add<"density">(f1);

// Not possible due to change in object type etc
v1 = std::move(v1).add<"density1">(f2);

// But instead the macro enables (not sure how useful):
v1 = VIS_REBIND(v1, "density2", f2){
  auto d2 = v1.get<"density2">();
}
```
- Added id-tag struct for alternative data retrieval:
```cpp
...
auto vis = MakeVisAdaptor<"E","rho","phi","density">(f3, F2, F3, f1);
/* alternative overwrite .. */
vis.set(id<"density">, f2);

auto reg = vis.get_registry();

std::cout << vis.get<"E">().data << std::endl;
std::cout << vis.get(id<"E">).data << std::endl;  
std::cout << reg.Get<"E">().data << std::endl;    
std::cout << reg.Get(id<"E">).data << std::endl;  
```

Notes
- `add<"ID">` warns and is a no-op when the ID already exists in the adaptor type.
- `set<"ID">` warns on rebinding and rebinds the pointer.

## src_dynamic 
Dynamic, type-safe registry and adaptor
<!-- 
Attempt more dynamic name-to-type mapping.
  - Approach: Runtime/type-erased registry. Stores base pointers (Field_b*, ParticleBase_b*) plus parallel type metadata: (type_hash, dim, vdim, is_vector, scalar_type_hash). Lookup by string ID. Visitors use a runtime scalar hash combined with a compile-time supported_types tuple to dispatch.
  - Pros: Flexible, easy to extend at runtime, simple lookup by ID.
  - Cons: Some runtime cost; relies on stored metadata correctness.
 -->

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

### VisAdaptorBase API (facade with a smart-pointer registry instance)
- `get_registry()`

Compile-time name-only API (requires registration):
- `add<"ID">(obj)`
- `get<"ID">() -> T&`
- `contains<"ID">() -> bool`
- `remove<"ID">() -> bool`

Runtime string API:
- `add_named(name, obj)`
- `get_named<T>(name) -> T*` (nullptr if absent or wrong type)
- `contains_named(name) -> bool`
- `remove_named(name) -> bool`

Support for visitor patterns can be added for both runtime and compile-time APIs.

### Examples
Compile-time API (type-checked):
```cpp
VisAdaptorBase vis;
Field<double, 1> density;
vis.add<"density">(density);
std::cout << vis.get<"density">().data << "\n"; 
```

Runtime string API (flexible):
```cpp
VisAdaptorBase vis;
int score = 42;
vis.add_named("score", score);
if (auto* s = vis.get_named<int>("score")) {
    std::cout << *s << "\n";
}
```

- TODOs:


## src_dynamic_auto (auto-registration with a single global instance)
Having a global registry and `VisAdaptorBase` instance would be possible with this approach, but we decided to avoid a global namespace instance for now.
- Approach: Dynamic registry with auto-registration. Field/particle construction hooks register instances automatically into the global registry (`VisBase`). Adds helpers like find-by-ID and print/visit utilities to reduce boilerplate.
- Pros: Minimal user code to participate in visualization; safer, more ergonomic dynamic approach.
- Cons: Care needed for construction order and lifetime; still runtime-based dispatch.



