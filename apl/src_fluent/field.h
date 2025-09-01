#pragma once
#include "bpl.h"

class Field_b{
public:
    std::string field_ID;
    virtual ~Field_b() = default;
    virtual size_t getTypeHash() const = 0;
    virtual size_t getDim() const = 0;
};

template<typename T, unsigned Dim>
class Field : public Field_b{
public:
    using value_type = T;
    vec<T, Dim> data;
    Field();
    Field(std::string name);
    Field(std::string name, T v);

    size_t getTypeHash() const override { return typeid(T).hash_code(); }
    size_t getDim() const override { return Dim; }
    const std::array<T, Dim>& getData() const { return data; }
    std::array<T, Dim>& getData() { return data; }
};

template<typename T, unsigned Dim>
Field<T,Dim>::Field(std::string name, T v)  : data({v}){
    field_ID = name;
    std::cout << "creating field container valued" << std::endl;
}

template<typename T, unsigned Dim>
Field<T,Dim>::Field(std::string name) {
    field_ID = name;
    fill_with_random(data);
    std::cout << "creating field container named" << name << std::endl;
}

template<typename T, unsigned Dim>
Field<T,Dim>::Field()   {
    field_ID = "Field<"+ std::string(typeid(T).name()).substr(0,1) + ","+std::to_string(Dim)+">_unlabeled";
    fill_with_random(data);
    std::cout << "creating field container (default)" << std::endl;
}
