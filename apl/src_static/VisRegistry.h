#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <initializer_list>


/////////////////////////////////////////////////////
// NOOOO STAAAAAATICS... minimize global ...
/////////////////////////////////////////////////////
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











// Compile-time string literal wrapper (C++20 NTTP).
template <std::size_t N>
struct fixed_string {
    char data[N]{};
    constexpr fixed_string(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) data[i] = str[i];
    }
    constexpr std::string_view sv() const { return std::string_view{data, N - 1}; }
};
template <std::size_t N, std::size_t M>
constexpr bool operator==(const fixed_string<N>& a, const fixed_string<M>& b) {
    if constexpr (N != M) return false;
    for (std::size_t i = 0; i < N; ++i) if (a.data[i] != b.data[i]) return false;
    return true;
}


// Slot<"id", T> declares one compile-time entry.
template <fixed_string IdV, typename T>
struct Slot {
    static constexpr auto Id = IdV;
    using type = T;


};

// nth type helper
template <std::size_t I, typename First, typename... Rest>
struct nth { using type = typename nth<I - 1, Rest...>::type; };
template <typename First, typename... Rest>
struct nth<0, First, Rest...> { using type = First; };

// index_of helper
template <auto IdV, std::size_t I, typename First, typename... Rest>
struct find_index_rec {
    static constexpr std::size_t value =
        (First::Id == IdV) ? I : find_index_rec<IdV, I + 1, Rest...>::value;
};
template <auto IdV, std::size_t I, typename Last>
struct find_index_rec<IdV, I, Last> {
    static constexpr std::size_t value = (Last::Id == IdV) ? I : static_cast<std::size_t>(-1);
};









// Typed, immutable registry keyed by compile-time IDs.
template <typename... Slots>
class RegistryTyped : public RegistryBase {



    // template <std::size_t I>
    // using SlotAt = typename nth<I, Slots...>::type;
    // template (std::size_t I) using TypeAt = typename SlotAt<I>::type;

    template <std::size_t I>
    using SlotAt = typename nth<I, Slots...>::type;
    template <std::size_t I> using TypeAt = typename SlotAt<I>::type;

    template <std::size_t I>
    static constexpr auto IdAt = SlotAt<I>::Id;

    std::tuple<typename Slots::type*...> m_ptrs{};
    std::unordered_set<std::string> m_names;


    
    
    public: // ==========================PRIVATE=============================
    
    using Entry = typename RegistryBase::Entry;
   

    RegistryTyped(std::initializer_list<Entry> entries) {
        std::unordered_map<std::string, std::any> tmp;
        tmp.reserve(entries.size());
        for (const auto& e : entries) {
            if (!tmp.emplace(e.name, e.ptr_any).second) {
                throw std::invalid_argument("Duplicate name provided to RegistryTyped: " + e.name);
            }
        }
        init_from_map(tmp);
    }

    // get<"id">() -> T& (no type argument needed)
    template <auto IdV>
    auto& Get() const {
        constexpr std::size_t I = index_of<IdV>();
        static_assert(I != static_cast<std::size_t>(-1), "Unknown ID in RegistryTyped");
        using SlotT = typename nth<I, Slots...>::type;
        using T = typename SlotT::type;
        T* ptr = std::get<I>(m_ptrs);
        if (!ptr) throw std::runtime_error(std::string("Null entry for ID: ") + std::string(SlotT::Id.sv()));
        return *ptr;
    }

    bool Exists(std::string_view name) const {
        return m_names.count(std::string(name)) != 0;
    }




    private: // ==========================PRIVATE=============================


    template <auto IdV>
    static constexpr std::size_t index_of() {
        return find_index_rec<IdV, 0, Slots...>::value;
    }



    void init_from_map(const std::unordered_map<std::string, std::any>& tmp) {
        m_names.clear();
        m_names.reserve(tmp.size());
        init_each(tmp, std::make_index_sequence<sizeof...(Slots)>{});
    }

    template <std::size_t... Is>
    void init_each(const std::unordered_map<std::string, std::any>& tmp, std::index_sequence<Is...>) {
        // Populate tuple in slot order, checking presence and type.
        (assign_one<Is>(tmp), ...);
    }

    template <std::size_t I>
    void assign_one(const std::unordered_map<std::string, std::any>& tmp) {
        const auto name_sv = IdAt<I>.sv();
        auto it = tmp.find(std::string(name_sv));
        if (it == tmp.end()) {
            throw std::invalid_argument("Missing required ID in RegistryTyped: " + std::string(name_sv));
        }
        try {
            std::get<I>(m_ptrs) = std::any_cast<TypeAt<I>*>(it->second);
            m_names.insert(it->first);
        } catch (const std::bad_any_cast&) {
            throw std::invalid_argument("Type mismatch for ID: " + it->first);
        }
    }
};

















/* ========================================================================= */
/* ========================================================================= */
/* ========================================================================= */


    // Factory: make a unique_ptr<RegistryTyped<Slot<Id_i, T_i>...>> from objects.
    // Usage:
    //   auto reg = MakeRegistry<"mesh","solver">(meshObj, solverObj);
    //   auto& solver = reg->Get<"solver">();




// requires ((!std::is_pointer_v<std::remove_reference_t<Ts>> && ...))
template <fixed_string... Ids, typename... Ts>
std::unique_ptr<RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>> MakeRegistry(Ts&... objs) {
        using Reg = RegistryTyped<Slot<Ids, std::remove_reference_t<Ts>>...>;
        return = std::make_unique<Reg>(
            std::initializer_list<typename RegistryImmutable::Entry>{
                typename RegistryImmutable::Entry{std::string(Ids.sv()), &objs}...
            }
        );
    }
    


// Optional pointer overload if you already have pointers.
template <fixed_string... Ids, typename... Ts>
std::unique_ptr<RegistryTyped<Slot<Ids, Ts>...>>  MakeRegistryPtrs(Ts*... ptrs) {
    using Reg = RegistryTyped<Slot<Ids, Ts>...>;
    return std::make_unique<Reg>(
        std::initializer_list<typename RegistryImmutable::Entry>{
            typename RegistryImmutable::Entry{std::string(Ids.sv()), ptrs}...
        }
    );
}



/* counting  */
//     virtual size_t getScalarFieldCount() const = 0;
//     virtual size_t getVectorFieldCount() const = 0;
//     virtual size_t getParticleCount() const = 0;
//     // Type-erased base overrides for counts and finds
//     size_t getScalarFieldCount() const override { return scalar_fields.size(); }
//     size_t getVectorFieldCount() const override { return vector_fields.size(); }
//     size_t getParticleCount() const override { return particles.size(); }



/*  Listing */
//     virtual void listAllFields() const = 0;
//     virtual void listParticles() const = 0;
//     // Finds (base-pointer returns to avoid template args)




/* get / find */
//     // Typed find functions that return proper types
//     Field<T, Dim>* findScalarFieldByID(const std::string& field_id);
//     Field<vec<T, Dim>, Dim>* findVectorFieldByID(const std::string& field_id);
//     ParticleBase<T, Dim>* findParticleByID(const std::string& bunch_id);
    
//     // General find that searches both scalar and vector fields
//     Field_b* findFieldByID(const std::string& field_id);
    
/*  visit *//* apply to all .. */

//     // Typed list helpers
//     void listScalarFields() const;
//     void listVectorFields() const;
//     // listAllFields and listParticles also override the base interface
//     void listAllFields() const override;
//     void listParticles() const override;


















