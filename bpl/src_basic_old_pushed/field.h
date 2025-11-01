#pragma once
#include <iostream>
#include <string>


class Field_b{
    public:
    std::string field_ID;
    bool is_registered = false;  // Track registration status
    
    // Check if this field was successfully registered
    bool isRegistered() const { return is_registered; }
};


template<typename T, unsigned Dim>
class Field : public Field_b{
  public:
    inline static int counter = 0;
    T data;
    

    Field();
    Field(std::string name, T v) ;

};



  

    template<typename T, unsigned Dim>
    Field<T,Dim>::Field(std::string name, T v)  : data(v){
        ++counter;
        field_ID = name;
        is_registered = false;  // Initialize as not registered
        std::cout << "creating field container" << std::endl;
        
        // Validate type before registering - catch exceptions internally
        try {
            if constexpr (is_vec<T>::value) {
                constexpr unsigned VDim = is_vec<T>::vdim;
                VisBase::validateVectorFieldType<T, Dim, VDim>();
            } else {
                VisBase::validateScalarFieldType<T, Dim>();
            }
            VisBase::sf_c.push_back(this);
            is_registered = true;  // Mark as successfully registered
        } catch (const std::runtime_error& e) {
            std::cout << "Field creation failed: " << e.what() << std::endl;
            std::cout << "Field '" << name << "' was not registered." << std::endl;
            // Field object is created but not registered in the manager
        }
    }

    // Field<T,Dim>::Field() :  Field("unnamed_"+ std::to_string(counter), 0.0)  { 
    template<typename T, unsigned Dim>
    Field<T,Dim>::Field()   { 
        ++counter;
        field_ID = "Field<"+ std::string(typeid(T).name()).substr(0,1) + ","+std::to_string(Dim)+">_"
                + std::to_string(counter)+"_unlabeled";
        is_registered = false;  // Initialize as not registered
        std::cout << "creating field container (default)" << std::endl;
        
        // Validate type before registering - catch exceptions internally
        try {
            if constexpr (is_vec<T>::value) {
                constexpr unsigned VDim = is_vec<T>::vdim;
                VisBase::validateVectorFieldType<T, Dim, VDim>();
            } else {
                VisBase::validateScalarFieldType<T, Dim>();
            }
            VisBase::sf_c.push_back(this);
            is_registered = true;  // Mark as successfully registered
        } catch (const std::runtime_error& e) {
            std::cout << "Field creation failed: " << e.what() << std::endl;
            std::cout << "Field '" << field_ID << "' was not registered." << std::endl;
            // Field object is created but not registered in the manager
        }
    }