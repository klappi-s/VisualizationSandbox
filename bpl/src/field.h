#pragma once

// Field contaner template
template<typename T>
class Field{
  public:

    
    inline static int counter = 0;
    std::string fcName;
    T value;

    

    Field();
    Field(std::string name, T v);
};


#include"field.hpp"