#pragma once

// ParticleBunch container template
template<typename T, unsigned Dim>
class ParticleBase {
  public:


    inline static int counter = 0;
    std::string bunch_ID;
    std::vector<std::array<T,Dim>> values;

    ParticleBase();
    ParticleBase(std::string name, std::array<T,3> v);
};


#include"particle.hpp"