#pragma once
#include <iostream>
#include <string>
#include <any>
#include <typeinfo>

#include "VIS_forward.h"

class Field_b{
    public:
    std::string field_ID;
    virtual ~Field_b() = default;  // Make it polymorphic
    
    // Virtual methods for type-safe access
    virtual std::any getData() const = 0;
    virtual size_t getTypeHash() const = 0;
    virtual size_t getDim() const = 0;
};


template<typename T, unsigned Dim>
class Field : public Field_b{
  public:
    using value_type = T;  // Add typedef for the stored type
    inline static int counter = 0;
    std::array<T, Dim> data;
    
    Field();
    Field(std::string name);
    Field(std::string name, T v);
    
    // Implement virtual methods from base class
    std::any getData() const override {
        return std::any(data);
    }
    
    size_t getTypeHash() const override {
        return typeid(T).hash_code();
    }

    size_t getDim() const override {
        return Dim;
    }
    
    // Access data safely
    // const std::array<T, Dim>& getValue() const { return data; }
    // std::array<T, Dim>& getValue() { return data; }
};



  

    template<typename T, unsigned Dim>
    Field<T,Dim>::Field(std::string name, T v)  : data({v}){
        ++counter;
        field_ID = name;
        std::cout << "creating field container" << std::endl;
        VisBase::sf_c.push_back(this);
        VisBase::registerFieldType<T, Dim>(this);
        // Add field_ID mapping after registration
        VisBase::field_id_map[field_ID] = VisBase::sf_c.size() - 1;
    }


    template<typename T, unsigned Dim>
    Field<T,Dim>::Field(std::string name) {
        ++counter;
        field_ID = name;
        fill_with_random(data);
        std::cout << "creating field container" << std::endl;
        VisBase::sf_c.push_back(this);
        VisBase::registerFieldType<T, Dim>(this);
        // Add field_ID mapping after registration
        VisBase::field_id_map[field_ID] = VisBase::sf_c.size() - 1;
    }



    // Field<T,Dim>::Field() :  Field("unnamed_"+ std::to_string(counter), 0.0)  { 
    template<typename T, unsigned Dim>
    Field<T,Dim>::Field()   { 
        ++counter;
        field_ID = "Field<"+ std::string(typeid(T).name()).substr(0,1) + ","+std::to_string(Dim)+">_"
                + std::to_string(counter)+"_unlabeled";
        fill_with_random(data);
        std::cout << "creating field container (default)" << std::endl;
        VisBase::sf_c.push_back(this);
        VisBase::registerFieldType<T, Dim>(this);
        // Add field_ID mapping after registration
        VisBase::field_id_map[field_ID] = VisBase::sf_c.size() - 1;
    }