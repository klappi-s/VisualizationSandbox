# APL: src_dynamic_auto — Runtime registry with auto-registration

## Overview
Dynamic registry that auto-registers fields and particle containers into a global registry during construction/initialization. Adds ergonomic helpers to discover, list, apply, and visit.

Status
- Code layout mirrors `src_dynamic` (Vis_forward.h, VisRegistry.h, VisBase.h/.hpp/.cpp) but uses a global registry and auto-registration hooks.
- README alignment pending. See examples and headers for authoritative API.

## Registry design approach
- Global registry (in VisBase) stores pointers with lightweight metadata.
- Objects register themselves (or via helper hooks) so user code has minimal boilerplate.
- Visit/apply helpers: apply_to_all_scalar_fields, apply_to_all_vector_fields, visit_field/visit_scalar_field/visit_vector_field.

## Files
- `Vis_forward.h`, `VisRegistry.h`, `VisBase.h/.hpp/.cpp`
- `field.h`, `particle.h`
- `amain.cpp`, `bdemo.cpp`, `Makefile`

## Build & Run
- Build: `make`
- Run demos: `make run` (amain) or `make run_bdemo`

---

## Variants overview
- `src_static`: compile-time/static registry. Types and dimensions encoded in templates; shared ownership and accessor parity.
- `src_dynamic`: runtime map-based registry with compile-time ID→type mapping and runtime string API.
- `src_dynamic_auto`: dynamic registry with auto-registration hooks (this module).
- `src_fluent`: compile-time fluent/builder adaptor over a typed registry; shared ownership and fluent adds.
