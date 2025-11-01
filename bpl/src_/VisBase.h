#pragma once
#include "VIS_forward.h"





//        Type information: (type_hash, Dim, VDim, is_vector, scalar_type_hash)
using TypeInfo = std::tuple<size_t, unsigned, unsigned, bool, size_t>;





// template<typename ST>
class VisBase{
    public:
    
    std::string vis_ID;
    virtual ~VisBase() = default;  // Virtual destructor to fix warning
    virtual int get_size_pb();
    virtual int get_size_sf();
    
    inline static std::vector<Field_b*> sf_c; 
    inline static std::vector<ParticleBase_b*> pb_c;
    
    // Type information storage parallel to sf_c
    inline static std::vector<TypeInfo> sf_type_info;
    inline static std::unordered_map<std::string, size_t> field_id_map;


    
    // Template helper for type registration - registers both type info and extraction functions
    template<typename FT, unsigned Dim>
    static void registerFieldType(Field_b* field_ptr);








    static bool hasField(const std::string& field_id);

    static Field_b* findFieldByID(const std::string& field_id);
    static ParticleBase_b* findParticleByID(const std::string& particle_id);

    
    static void printFieldInfo(const std::string& field_id);
    static void PrintListFields();


    template<typename T, unsigned Dim>
    static std::vector<std::string> getListFieldsOfType_ID();


    // Helper to get type information string
    static std::string getFieldTypeInfo(const std::string& field_id);
    




    // ====== AUTO-TYPED FIELD ACCESS ======

    // getFieldRef_byType: Returns reference to Field object with exact type match  
    template<typename FT, unsigned Dim>
    static std::optional<std::reference_wrapper<Field<FT, Dim>>> getFieldRef_byType(const std::string& field_id);
    




    // Simple auto-detection method - returns correctly typed pointer without template parameters
    /*  but with void* therefore kinda useles...  */
    static void* getTypedField(const std::string& field_id);
    
    // Template wrapper for convenience
    template<typename T>
    static T* getFieldAs(const std::string& field_id) {
        void* ptr = getTypedField(field_id);
        return static_cast<T*>(ptr);
    }


   


    

    // ====== APPLY TO ALL  ======
    // Apply function to all scalar fields
    template<typename Func>
    static void apply_to_all_scalar_fields(Func&& func);
    
    // Apply function to all vector fields
    template<typename Func>
    static void apply_to_all_vector_fields(Func&& func);


    // Apply function to all fields (both scalar and vector)
    template<typename Func>
    static void apply_to_all_fields(Func&& func);




    

    // ====== VISITORS  ======
    /* could be done recursive
    more DRY nice but probably illegible */
    using supported_types = std::tuple<double, float, int>;

        // Generic visitor dispatch helper
    template<typename Visitor, typename ScalarType>
    static auto visit_field_of_scalar_type(Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor);


    // Base case: Ran out of types in the tuple, so the type is unsupported.
    template<typename Visitor>
    static auto visit_field_dispatcher(size_t scalar_hash, const std::string& field_id, Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor, std::tuple<>);

    // Recursive step: Check the first type in the tuple.
    template<typename Visitor, typename Head, typename... Tail>
    static auto visit_field_dispatcher(size_t scalar_hash, const std::string& field_id, Field_b* field_ptr, unsigned dim, unsigned vdim, bool is_vector, Visitor&& visitor, std::tuple<Head, Tail...>);


    template<typename Visitor, typename... ScalarTypes>
    static auto visit_field_impl(const std::string& field_id, bool expect_vector, Visitor&& visitor, std::tuple<ScalarTypes...>); 


    template<typename Visitor>
    static auto visit_field_impl_2(const std::string& field_id, bool expect_vector, Visitor&& visitor);


    // Visit specific scalar field by ID
    template<typename Visitor>
    static auto visit_scalar_field(const std::string& field_id, Visitor&& visitor);
    
    // Visit specific vector field by ID
    template<typename Visitor>
    static auto visit_vector_field(const std::string& field_id, Visitor&& visitor);
    
    // Generic visit (auto-detects field type)
    template<typename Visitor>
    static auto visit_field(const std::string& field_id, Visitor&& visitor);










};
#include "VisBase.hpp"





