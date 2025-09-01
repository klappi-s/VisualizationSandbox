# APL: src_fluent — Compile-time fluent adaptor

## Overview
Compile-time registry variant with a fluent/builder-style adaptor. Adds ergonomic add<"ID"> chaining while preserving static guarantees.

## Registry design approach
- `RegistryFluent<Slots...>` stores typed pointers; compile-time ID→index mapping.
- `VisAdaptorBase<>` supports `.add<"ID">(obj)` producing a new adaptor type with accumulated Slots.
- Helpers: `id_tag`/`id<...>`, factories to create from refs/pointers/shared/unique_ptr.

## Files
- `Vis_forward.h`, `VisRegistry.h` (RegistryFluent), `VisBase.h` (fluent adaptor)
- `field.h`, `particle.h`
- `amain.cpp`, `bdemo.cpp`, `Makefile`

## Build & Run
- Build: `make`
- Run demos: `make run` (amain) or `make run_bdemo`

---

## Details

Fluent, compile-time adaptor/registry variant. Mirrors `src_static` layout and APIs, with a builder-style `VisAdaptorBase<>` that accumulates slots via `add<"ID">(obj)` returning a new adaptor type.

Highlights
- Builder API: `VisAdaptorBase<>` starts empty; `.add<"ID">(obj)` returns a new type `VisAdaptorBase<Slots..., Slot<"ID", T>>` and carries previously bound pointers forward.
- Rebind semantics: Registry warns on rebinding for `set`/`set_ptr`. `add` warns and no-ops if the ID already exists in the adaptor type.
- Shared ownership: `VisAdaptorBase` holds `std::shared_ptr<RegistryFluent<...>>`, can be constructed from an existing `shared_ptr` or promoted from a `unique_ptr`.
- Tag-based convenience: Use `id<"density">` to call `get(id<...>)`, `set(id<...>, ...)`, avoiding the `template` keyword in dependent contexts.
- Factories: `MakeRegistry<Ids...>(refs...)`, `MakeRegistryPtrs<Ids...>(ptrs...)` produce `unique_ptr<RegistryFluent<...>>`. `MakeVisAdaptor(objs...)` or `MakeVisAdaptor(shared_ptr/unique_ptr)` to create adaptors.

What’s different from src_static
- Registry type: `RegistryFluent` vs `RegistryTyped`.
  - No `m_names` tracking; tuple-of-pointers storage.
  - Same compile-time ID→index mapping and `Get/Set/SetPtr/Contains/Unset` with both template and tag overloads.
- Adaptor API: fluent `add<"ID">` returns a new adaptor type, enabling type-level accumulation of Slots.
- Extra ergonomics: `id_tag`/`id<>`, helpers `add_slot`, `with_added`, and `VIS_REBIND` macro for convenient chaining and name reuse.
- Ownership/interop: Shared ownership and accessors: `get_registry()` / `get_registry_ptr()`.

Code layout (mirrors src_static)
- `Vis_forward.h`: sectioned utilities, compile-time IDs, tags, and aggregates public headers. Comments call out fluent-specific differences.
- `VisRegistry.h`: `RegistryFluent<Slots...>` with tuple storage and compile-time ID mapping. Rebind warnings for `Set/SetPtr`. Factories for creating registries. Template APIs use `template<fixed_string IdV>` consistently.
- `VisBase.h`: `VisAdaptorBase<Slots...>` fluent builder and thin wrapper over `RegistryFluent`. Shared_ptr ownership, constructors from existing registries, and accessors.
- `field.h`, `particle.h`: demo data types; same as `src_static`.
- `amain.cpp`, `bdemo.cpp`: show chaining with `add`, tag-based `get`, and `VIS_REBIND` usage.

Usage examples
- Start empty and add slots fluently:
  ```cpp
  VisAdaptorBase<> vis;
  Field<double,1> rho;
  auto vis2 = std::move(vis).add<"density">(rho);
  std::cout << vis2.get<"density">().data << "\n";
  ```
- From existing registry (unique -> adaptor shares promoted registry):
  ```cpp
  auto reg = MakeRegistry<"rho", "phi">(rho, phi); // unique_ptr<RegistryFluent<...>>
  auto vis = MakeVisAdaptor(reg);                    // promote to shared_ptr and wrap
  std::cout << vis.get(id<"rho">).data << "\n";
  ```
- Tag-based API:
  ```cpp
  vis.get(id<"rho">);
  vis.set(id<"rho">, rho2);
  ```

Build
- `make` in `src_fluent/` builds `bild/amain` and `bild/bdemo`.

Notes
- `add<"ID">` warning + no-op when the ID already exists in the adaptor type.
- `set<"ID">` and `set_ptr<"ID">` warn on rebinding and rebind the pointer.
- This variant focuses on compile-time accumulation and ergonomics; for dynamic name-to-type mapping, see `src_dynamic`.

---

## Variants overview
- `src_static`: compile-time/static registry. Types and dimensions encoded in templates; zero/low runtime overhead; maximal type safety.
- `src_dynamic`: runtime map-based registry with compile-time ID→type mapping and runtime string API.
- `src_dynamic_auto`: dynamic registry with auto-registration hooks. Constructors/initializers register into a global registry; helpers for find/apply/visit.
- `src_fluent`: compile-time fluent/builder adaptor. Ergonomic add<"ID"> chaining over a typed registry; same static guarantees as `src_static`.
