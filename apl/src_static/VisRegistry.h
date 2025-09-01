#pragma once
#include "Vis_forward.h"




class RegistryBase{

    public:

    virtual ~RegistryBase() = default;

    struct Entry {
        std::string name;
        std::any ptr_any;

        template <typename T>
        Entry(const std::string& entry_name, T* ptr)
            : name(entry_name), ptr_any(ptr) {}
    };
};




// Typed registry keyed by compile-time IDs; supports late binding.
template <typename... Slots>
class RegistryTyped : public RegistryBase {

    template <std::size_t I>
    using SlotAt = typename nth<I, Slots...>::type;
    template <std::size_t I> using TypeAt = typename SlotAt<I>::type;

    template <std::size_t I>
    static constexpr auto IdAt = SlotAt<I>::Id;

    // New: constexpr index as a variable template (usable in type contexts)
    template <auto IdV>
    static constexpr std::size_t index_of_v = find_index_rec<IdV, 0, Slots...>::value;

    std::tuple<typename Slots::type*...> m_ptrs{};      // nullptr when not bound
    std::unordered_set<std::string>      m_names;       // names that are currently bound

public:
    using Entry = typename RegistryBase::Entry;

    RegistryTyped(std::initializer_list<Entry> entries) {
        std::unordered_map<std::string, std::any> tmp;
        tmp.reserve(entries.size());
        for (const auto& e : entries) {
            // last one wins (allows construction with duplicates intentionally)
            tmp[e.name] = e.ptr_any;
        }
        init_from_map(tmp);
    }

       // get<"id">() -> T& (throws if not bound)
    template <fixed_string IdV>
    auto& Get() const {
        constexpr std::size_t I = index_of_v<IdV>;
        static_assert(I != static_cast<std::size_t>(-1), "Unknown ID in RegistryTyped");
        using SlotT = typename nth<I, Slots...>::type;
        using T = typename SlotT::type;
        T* ptr = std::get<I>(m_ptrs);
        if (!ptr) throw std::runtime_error(std::string("Null entry for ID: ") + std::string(SlotT::Id.sv()));
        return *ptr;
    }

    // Bind/replace an entry after construction with compile-time ID (by reference)
    template <fixed_string IdV, typename U>
    void Set(U& object) {
        constexpr std::size_t I = index_of_v<IdV>;
        static_assert(I != static_cast<std::size_t>(-1), "Unknown ID in RegistryTyped::Set");
        using T = TypeAt<I>;
        static_assert(std::is_same_v<std::remove_const_t<U>, T>,
                      "Type mismatch in RegistryTyped::Set for this ID");
        std::get<I>(m_ptrs) = &object;
        m_names.insert(std::string(SlotAt<I>::Id.sv()));
    }

    // Pointer variant
    template <fixed_string IdV>
    void SetPtr(TypeAt<index_of_v<IdV>>* ptr) {
        constexpr std::size_t I = index_of_v<IdV>;
        std::get<I>(m_ptrs) = ptr;
        if (ptr) m_names.insert(std::string(SlotAt<I>::Id.sv()));
        else     m_names.erase(std::string(SlotAt<I>::Id.sv()));
    }

    // Check binding
    template <fixed_string IdV>
    bool Contains() const {
        constexpr std::size_t I = index_of_v<IdV>;
        static_assert(I != static_cast<std::size_t>(-1), "Unknown ID in RegistryTyped::Contains");
        return std::get<I>(m_ptrs) != nullptr;
    }

    // Optional: remove/unbind
    template <fixed_string IdV>
    void Unset() {
        constexpr std::size_t I = index_of_v<IdV>;
        std::get<I>(m_ptrs) = nullptr;
        m_names.erase(std::string(SlotAt<I>::Id.sv()));
    }

private:

    void init_from_map(const std::unordered_map<std::string, std::any>& tmp) {
        m_names.clear();
        m_names.reserve(tmp.size());
        init_each(tmp, std::make_index_sequence<sizeof...(Slots)>{});
    }

    template <std::size_t... Is>
    void init_each(const std::unordered_map<std::string, std::any>& tmp, std::index_sequence<Is...>) {
        (assign_one<Is>(tmp), ...);
    }

    template <std::size_t I>
    void assign_one(const std::unordered_map<std::string, std::any>& tmp) {
        const auto name_sv = IdAt<I>.sv();
        auto it = tmp.find(std::string(name_sv));
        if (it == tmp.end()) {
            // Allow missing; leave nullptr and not present in m_names
            std::get<I>(m_ptrs) = nullptr;
            return;
        }
        try {
            std::get<I>(m_ptrs) = std::any_cast<TypeAt<I>*>(it->second);
            if (std::get<I>(m_ptrs)) m_names.insert(it->first);
        } catch (const std::bad_any_cast&) {
            throw std::invalid_argument("Type mismatch for ID: " + it->first);
        }
    }
};







// Factories 
template <fixed_string... Ids, typename... Ts>
std::unique_ptr<RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>> MakeRegistry(Ts&... objs) {
    using Reg = RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>;
    return std::make_unique<Reg>(
        std::initializer_list<typename RegistryBase::Entry>{
            typename RegistryBase::Entry{std::string(Ids.sv()), &objs}...
        }
    );
}

template <fixed_string... Ids, typename... Ts>
std::unique_ptr<RegistryTyped<Slot<Ids, Ts>...>>  MakeRegistryPtrs(Ts*... ptrs) {
    using Reg = RegistryTyped<Slot<Ids, Ts>...>;
    return std::make_unique<Reg>(
        std::initializer_list<typename RegistryBase::Entry>{
            typename RegistryBase::Entry{std::string(Ids.sv()), ptrs}...
        }
    );
}