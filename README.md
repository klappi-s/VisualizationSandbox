# VisualizationSandbox

I shortly impemented a basic variant of th ippl layout, to be able to try out differents things on how to ideally integrate the visualisation; and get back into the cpp language, templats and metaprogramming...


# APL (a particle layer)



## Variants: Registry design approaches

This repo contains four source variants exploring different registry designs for fields and particle containers possibly used by the visualization layer.

The Registry does the main lifting and the VisAdaptorBase is an additional facace with a smart pointer to a registry instance. Future Adaptor varant can inherit from this Base

Facade like operators enabling the managing of the underlying Registry via the AdaptorBase could also be left out.

for all src folders one can run:
- make amain
- make run_amain
- make bdemo
- make run_bdemo

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
- get/set/contains/unset apply identical operation to undelying registry 

- `get_registry()` Returns a reference to the bound object. Throws if not bound.

- Can either create VisObject directly with Factory helper (like registry factory) and creates underlying registry by itself: `MakeVisAdaptor(Ts&... objs)
    -> VisAdaptorBase<Slot<Ids, std::remove_reference_t<Ts>>...>` 

- or alternative factory from existing smart pointer to registry with shared ownership: `VisAdaptorBase` holds `std::shared_ptr<RegistryTyped<...>>`, can be constructed from an existing `shared_ptr` or promoted from a `unique_ptr`.





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
Nearly identical to src_static + some extension. But enables fluent accumulation of new slots.

Fluent, compile-time adaptor/registry variant. Mirrors `src_static` layout and APIs. Can start completely empty `VisAdaptorBase<>` or with factory helper. `MakeRegistry<Ids...>(objs...)` and later accumulate more slots via `add<"ID">(obj)` returning a new adaptor type.


Usage examples
- Start empty and add slots fluently:
         
```cpp
         VisAdaptorBase<> vis;
         Field<double,1> rho;
         auto vis2 = std::move(vis).add<"density">(rho);
         std::cout << vis2.get<"density">().data << "\n";
```
- Macro helper: `VIS_REBIND` enables accumulating in virtually the same identifyer in followin scope (not ideal)

```cpp

    Field<double, 1> f1, f2;
    VisAdaptorBase<> v0;

    // normally need to change Adaptor name...
    auto v1 = std::move(v0).add<"density">(f1);

    // not possible due to change in object type etc
     v1 = std::move(v1).add<"density1">(f2);

    // but instead MACRO enables, not sure if too useful...:
     v1 = VIS_REBIND(v1, "density2", f2){
       auto d2 = v1.get("desity2");
     }
```
- Added id tag struct for data alternativ data retrieval:
```cpp
    ....
    auto vis = MakeVisAdaptor<"E","rho","phi","density">(f3, F2, F3, f1);
    /* alternative ovewrite .. */
    vis.set(id<"density">, f2);

    auto reg = vis.get_registry();
    
    std::cout << vis.get<"E">().data << std::endl;
    std::cout << vis.get(id<"E">).data << std::endl;  
    std::cout << reg.Get<"E">().data << std::endl;    
    std::cout << reg.Get(id<"E">).data << std::endl;  
```


Notes
- `add<"ID">` warning + no-op when the ID already exists in the adaptor type.
- `set<"ID">`  warn on rebinding and rebind the pointer.


## src_dynamic 
Dynamic Type-Safe Registry and VisAdaptor
<!-- 
Attempt more dynamc name to type mapping.
  - Approach: Runtime/type-erased registry. Stores base pointers (Field_b*, ParticleBase_b*) plus parallel type metadata: (type_hash, dim, vdim, is_vector, scalar_type_hash). Lookup by string ID. Visitors use a runtime scalar hash combined with a compile-time supported_types tuple to dispatch.
  - Pros: Flexible, easy to extend at runtime, simple lookup by ID.
  - Cons: Some runtime cost; relies on stored metadata correctness.
 -->


This module provides a minimal, type-safe registry that binds compile-time string IDs to concrete C++ types, plus a small facade (`VisAdaptorBase`) for easy use. It supports both compile-time (name-only) and runtime string APIs.

### Key Concepts
- Registry and Adaptor are not a template!!
- Registry maps "well-known" compile-time IDs (like "density") to types via a registration macro.
- At runtime you bind objects to those IDs and retrieve them with full static type safety.
- A separate runtime string API allows ad-hoc storage/retrieval with explicit types.

### Registration (ID → Type)
Declare the mapping once at namespace scope:
Specialises a mapping struct template, defined inside the registry class:

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
- Set<"ID">(obj)
- Get<"ID">() -> T&
- Contains<"ID">() -> bool
- Unset<"ID">() -> bool
- could also include reg.get(id<"name">) syntax if needed

Runtime string API:
- add_named(name, obj)
- get_named<T>(name) -> T* (nullptr if absent or wrong type)
- contains_named(name) -> bool
- remove_named(name) -> bool


### VisAdaptorBase API (till now only a facade with smart pointer registry instance)
  
- get_registry()

Compile-time name-only API (requires registration):
- add<"ID">(obj)
- get<"ID">() -> T&
- contains<"ID">() -> bool
- remove<"ID">() -> bool

Runtime string API:
- add_named(name, obj)
- get_named<T>(name) -> T* (nullptr if absent or wrong type)
- contains_named(name) -> bool
- remove_named(name) -> bool

Can include visitor patterns for runtime and compile time API. On both levels


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






## src_dynamic_auto (for auto registration with single global instance)
Have global registry and VisAdaptor instance. Would be posible with this approach but we decided to avoid global amespace instance for now ...?
  - Approach: Dynamic registry with auto-registration. Field/particle construction hooks register instances automatically into the global registry (VisBase). Adds helpers like find by ID and print/visit utilities to reduce boilerplate.
  - Pros: Minimal user code to participate in visualization; safer, more ergonomic dynamic approach.
  - Cons: Care needed for construction order and lifetime; still runtime-based dispatch.



