# APL: src_fluent — Compile-time typed registry with a fluent adaptor

## Overview
This module provides a compile-time/static registry where IDs and types are encoded in templates, plus a fluent/builder-style `VisAdaptorBase<>` that makes it easy to accumulate bindings while preserving static guarantees. It is a complete, standalone variant.

## Rationale and approach
- Registry and adaptor are templated over slots of the form `Slot<Id, T>` where `Id` is a compile-time string and `T` is the bound type.
- ID→slot mapping, lookup, and dispatch are resolved at compile time.
- The adaptor keeps a `std::shared_ptr` to the underlying registry and exposes access to it via `get_registry()`; call registry APIs directly.
- The fluent API lets you start empty and `.add<"ID">(obj)` to get a new adaptor type that includes the new slot.

Pros
- Zero/low runtime overhead, maximal type safety.
- Ergonomic accumulation of bindings via the fluent builder.

Cons
- Adding new IDs/types requires recompilation; not meant for runtime/plugin-based extension.

## Files
- `Vis_forward.h`: utilities, compile-time IDs/tags, and public header aggregation.
- `VisRegistry.h`: `RegistryFluent<Slots...>` implementation with tuple-of-pointers storage and compile-time ID→index mapping.
- `VisBase.h`: `VisAdaptorBase<Slots...>` fluent builder and thin wrapper over the registry.
- `field.h`, `particle.h`: demo data types.
- `amain.cpp`, `bdemo.cpp`, `Makefile`.

## Build & Run
From `src_fluent/`:
- Build: `make`
- Run demos:
  - `make run` (amain)
  - `make run_bdemo`

## API

### RegistryFluent
Construction (factories)
- `MakeRegistry<Ids...>(objs...) -> std::unique_ptr<RegistryFluent<Slot<Ids, std::remove_reference_t<Ts>>...>>`
  - Binds each `Id` to the address of the corresponding `obj`.

Operations
- `Get<Id>() -> T&` / `Get(id<Id>) -> T&`
- `Set<Id>(U& obj)` / `Set(id<Id>, U& obj)`
- `Contains<Id>() -> bool` / `Contains(id<Id>) -> bool`
- `Unset<Id>()` / `Unset(id<Id>)`

Notes
- Stores raw pointers to user-owned objects; no ownership of the data.
- Unknown IDs result in `static_assert`.

### VisAdaptorBase (fluent)
- Holds a `std::shared_ptr` to a `RegistryFluent<...>`.
- Use `get_registry()` to access the full registry API: `get_registry().template Get<"ID">()`, `Set`, `Contains`, `Unset` (including tag-based overloads on the registry).
- Factories:
  - `MakeVisAdaptor<Ids...>(Ts&... objs) -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>`
  - Construct from existing `shared_ptr` or promote from `unique_ptr`.
- Fluent accumulation:
  - `.add<"ID">(obj)` returns a new adaptor type including that slot. Note: `add` is rvalue-qualified; call as `std::move(vis).add<...>(obj)` or use the helpers (`add_slot`, `with_added`, `VIS_REBIND`).
- Access to the underlying registry:
  - `get_registry()` returns a reference; `get_registry_ptr()` returns the shared pointer.

## Examples

Create data and an adaptor with a new registry
```cpp
Field<vec<double,3>, 3> F1;
Field<vec<double,2>, 2> F2;
Field<vec<double,1>, 1> F3;
Field<double, 1> f1, f2;

auto vis = MakeVisAdaptor<"E","rho","phi","density">(F1, F2, F3, f1);
auto& reg = vis.get_registry();

auto& E = reg.Get<"E">();
auto& rho = reg.Get<"rho">();

reg.Set<"density">(f2); // late rebind
```

Start empty and add fluently
```cpp
VisAdaptorBase<> v0;
Field<double,1> rho_f;
auto v1 = std::move(v0).add<"density">(rho_f);
std::cout << v1.get_registry().template Get<"density">().data << "\n";
```

Build adaptor from an existing registry
```cpp
auto reg2 = MakeRegistry<"E","rho">(F1, F2);   // unique_ptr
auto vis2 = MakeVisAdaptor(reg2);                  // promote to shared_ptr
auto& E2 = vis2.get_registry().template Get<"E">();
```

Tag-based convenience
```cpp
std::cout << vis.get_registry().Get(id<"E">).data << std::endl;
std::cout << reg.Get(id<"E">).data << std::endl;
```

Notes
- `add<"ID">` issues a warning and is a no-op if the ID already exists in the adaptor type.
- `Set<"ID">` on the registry warns on rebinding and rebinds the pointer.

## Related variants
If you need runtime flexibility instead of compile-time typing:
- See `../src_dynamic` for a runtime map-based registry with compile-time ID→type mapping for the compile-time API plus a runtime string API.
- See `../src_dynamic_auto` for a dynamic registry with auto-registration hooks.
