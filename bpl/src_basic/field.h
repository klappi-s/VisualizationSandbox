#pragma once
#include <iostream>
#include <string>


class Field_b{
    public:
    std::string field_ID;

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
        std::cout << "creating field container" << std::endl;
        
        // Validate type before registering
        if constexpr (is_vec<T>::value) {
            constexpr unsigned VDim = is_vec<T>::vdim;
            VisBase::validateVectorFieldType<T, Dim, VDim>();
        } else {
            VisBase::validateScalarFieldType<T, Dim>();
        }
        
        VisBase::sf_c.push_back(this);
    }

    // Field<T,Dim>::Field() :  Field("unnamed_"+ std::to_string(counter), 0.0)  { 
    template<typename T, unsigned Dim>
    Field<T,Dim>::Field()   { 
        ++counter;
        field_ID = "Field<"+ std::string(typeid(T).name()).substr(0,1) + ","+std::to_string(Dim)+">_"
                + std::to_string(counter)+"_unlabeled";
        std::cout << "creating field container (default)" << std::endl;
        
        // Validate type before registering
        if constexpr (is_vec<T>::value) {
            constexpr unsigned VDim = is_vec<T>::vdim;
            VisBase::validateVectorFieldType<T, Dim, VDim>();
        } else {
            VisBase::validateScalarFieldType<T, Dim>();
        }
        
        VisBase::sf_c.push_back(this);
    }