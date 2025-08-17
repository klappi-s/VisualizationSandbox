#pragma once
// #include "manager.hpp"

// ParticleBase template
template<typename T>
class ParticleBase {
  public:


    inline static int counter = 0;
    std::string pcName;
    std::vector<T> values;

    ParticleBase();
    ParticleBase(std::string name);
};


#include"particle.hpp"