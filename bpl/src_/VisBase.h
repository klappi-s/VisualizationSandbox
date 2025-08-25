#pragma once
#include <tuple>
#include <typeinfo>
#include <any>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>

// Forward declarations
class Field_b;
class ParticleBase_b;

template<typename T, unsigned Dim>
class Field;

// Type information: (type_hash, Dim, VDim, is_vector, scalar_type_hash)
using TypeInfo = std::tuple<size_t, unsigned, unsigned, bool, size_t>;

// Function type for data extraction
using DataExtractor = std::function<std::any(Field_b*)>;

// Function type for field creation
using FieldCreator = std::function<std::unique_ptr<Field_b>(const std::any&, const std::string&)>;

class VisBase{
    public:
    
    std::string vis_ID;
    virtual int get_size_pb(){return pb_c.size(); }
    virtual int get_size_sf(){return sf_c.size(); }
    
    inline static std::vector<Field_b*> sf_c; 
    inline static std::vector<ParticleBase_b*> pb_c;
    
    // Type information storage parallel to sf_c
    inline static std::vector<TypeInfo> sf_type_info;
    inline static std::unordered_map<std::string, size_t> field_id_map;
    
    // Type-specific function registries
    inline static std::unordered_map<size_t, DataExtractor> data_extractors;
    inline static std::unordered_map<size_t, FieldCreator> field_creators;
    
    // Field retrieval and recreation methods
    static Field_b* findFieldByID(const std::string& field_id);
    static std::any getFieldData(const std::string& field_id);
    static std::unique_ptr<Field_b> recreateField(size_t index);
    static void printFieldInfo(const std::string& field_id);
    static void listAllFields();
    
    // Template helper for type registration - registers both type info and extraction functions
    template<typename T, unsigned Dim>
    static void registerFieldType(Field_b* field_ptr);
    
private:
    // Internal helper method for registering type handlers
    static void registerTypeHandlers(size_t type_hash, DataExtractor extractor, FieldCreator creator);
};










