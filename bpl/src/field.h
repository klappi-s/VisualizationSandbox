#pragma once

// Field contaner template
template<typename T, unsigned Dim>
class Field{
  public:

    
    inline static int counter = 0;
    std::string field_ID;
    T value;

    

    Field();
    Field(std::string name, T v);
};


#include"field.hpp"