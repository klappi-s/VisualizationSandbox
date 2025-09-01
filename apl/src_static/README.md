# APL: src_static — Compile-time registry (typed)

## Overview
Compile-time typed registry using fixed_string IDs and Slot<> descriptions. All checks and dispatch resolved at compile time.

## Registry design approach
- IDs are non-type template parameters: fixed_string
- Compile-time mapping Slot<Id, T>; RegistryTyped<Slots...> stores typed pointers
- Adaptor VisAdaptorBase<Slots...> wraps/shares the registry and exposes helpers

## Files
- VisRegistry.h (fixed_string, Slot, RegistryTyped)
- VisBase.h (VisAdaptorBase)
- field.h, particle.h, Vis_forward.h
- amain.cpp, bdemo.cpp, Makefile

## Build & Run
- Build: make
- Run demos: make run (amain) or make run_bdemo

---

## Details

# Visualization Registry and Adaptor (VisRegistry + VisBase)

A small, type-safe registry that binds compile-time IDs to runtime objects by pointer and a lightweight adaptor that exposes a friendly API for visualization code.

## Core Concepts
- fixed_string: a compile-time string literal wrapper used as non-type template parameters (NTTPs).
- Slot<Id, T>: declares a compile-time entry with ID and value type T.
- RegistryTyped<Slots...>: holds pointers to bound objects for each slot.
- VisAdaptorBase<Slots...>: wraps a registry instance (shared_ptr) and provides convenient helpers.

IDs are known at compile time, so operations like Get<"id"> and Set<"id"> are checked at compile time for existence and type safety.

## Files
- `VisRegistry.h`: fixed_string, Slot, and the `RegistryTyped` implementation.
- `VisBase.h`: `VisAdaptorBase` that owns/shares a `RegistryTyped` and exposes a simple API.
- `amain.cpp`: example usage.

---

## VisRegistry API (RegistryTyped)
Template: `template <typename... Slots> class RegistryTyped` where each slot is `Slot<Id, T>`.

Construction
- From initializer list of entries (used by factories):
  - Each entry maps a name (string) to a typed pointer via `std::any`.
- Factories (helpers):
  - `MakeRegistry<Ids...>(objs...) -> std::unique_ptr<RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>>`
    - Binds each `Id` to the address of the corresponding `obj`.
  - `MakeRegistryPtrs<Ids...>(ptrs...) -> std::unique_ptr<RegistryTyped<Slot<Ids, Ts>...>>`
    - Binds each `Id` directly to the provided pointer.

Operations
- `Get<Id>() -> T&`
  - Returns a reference to the bound object. Throws if not bound.
- `Set<Id>(U& obj)`
  - Late-binds/rebinds by reference. Statically checks that `U` matches the slot type `T`.
- `SetPtr<Id>(T* ptr)`
  - Late-binds/rebinds by pointer. Unsets when `ptr == nullptr`.
- `Contains<Id>() -> bool`
  - True if the slot is currently bound (non-null pointer).
- `Unset<Id>()`
  - Removes a binding for the given Id.

Notes
- Internally stores raw pointers to user-owned objects. You are responsible for object lifetimes.
- Strong compile-time checks: unknown Ids trigger `static_assert`.

---

## VisAdaptor API (VisBase)
Template: `template <typename... Slots> class VisAdaptorBase` with `registry_t = RegistryTyped<Slots...>`.

Ownership
- Holds `std::shared_ptr<registry_t>` so multiple adaptors (or other code) can share the same registry object.

Constructors
- `VisAdaptorBase(Slots::type&... objs)`
  - Builds a new registry and binds each slot to the given object references.
- `VisAdaptorBase(Slots::type*... ptrs)`
  - Builds a new registry and binds each slot to the given pointers.
- `VisAdaptorBase(registry_t& r_init)`
  - Copy-constructs a new registry from an existing one (separate object, shared_ptr owner).
- `VisAdaptorBase(std::shared_ptr<registry_t> reg)`
  - Shares ownership of an existing registry object.
- `VisAdaptorBase(std::unique_ptr<registry_t>& reg)`
  - Promotes a unique_ptr registry to shared ownership (moves into shared_ptr).

Accessors
- `registry_t& get_registry()` and `const registry_t& get_registry() const`
  - Return a reference to the underlying registry object.
- `std::shared_ptr<registry_t> get_registry_ptr() const`
  - Returns the shared_ptr if you need to share/keep it elsewhere.

Helpers (forward to the registry)
- `get<Id>() -> T&`
- `set<Id>(U& obj)`
- `set_ptr<Id>(T* ptr)`
- `contains<Id>() -> bool`

Factories
- `MakeVisAdaptor<Ids...>(objs...) -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>`
  - Create an adaptor and its underlying registry in one call.
- Overloads:
  - `MakeVisAdaptor(std::shared_ptr<RegistryTyped<Slots...>>)`
  - `MakeVisAdaptor(std::unique_ptr<RegistryTyped<Slots...>>& )` (promotes to shared_ptr)

---

## Example
```cpp
// Create data
Field<vec<double,3>, 3> F1;
Field<vec<double,2>, 2> F2;
Field<vec<double,1>, 1> F3;
Field<double, 1> f1, f2;

// Create adaptor with new registry
auto vis = MakeVisAdaptor<"E","rho","phi","density">(F1, F2, F3, f1);

// Access via registry reference
auto& reg = vis.get_registry();
std::cout << reg.Get<"E">().data << std::endl;

// Or access through the adaptor
std::cout << vis.get<"rho">().data << std::endl;

// Late bind/overwrite
vis.set<"density">(f2);

// Build adaptor from an existing registry
auto reg2 = MakeRegistry<"E","rho">(F1, F2);            // unique_ptr
auto vis2 = MakeVisAdaptor(reg2);                           // promoted to shared_ptr
std::cout << vis2.get<"E">().data << std::endl;
```

---

## Build
- `make` or `make run` (see Makefile). Examples: `amain`, `bdemo`.

## Lifetime and Safety
- Registry stores raw pointers to external objects. Keep those objects alive while registered.
- `shared_ptr` is used only for the registry object itself; it does not manage the lifetimes of the bound data objects.

---

## Variants overview
- src_static: compile-time/static registry. Types and dimensions encoded in templates; zero/low runtime overhead; maximal type safety.
- src_dynamic: runtime map-based registry with compile-time ID→type mapping and runtime string API.
- src_dynamic_auto: dynamic registry with auto-registration hooks. Constructors/initializers register into a global registry; helpers for find/apply/visit.
- src_fluent: compile-time fluent/builder adaptor. Ergonomic add<"ID"> chaining over a typed registry; same static guarantees as src_static.
