#pragma once
#include <iostream>
#include <string>



class ParticleBase_b{
    public:
    std::string bunch_ID;
 
};

template<typename T, unsigned Dim>
class ParticleBase : public ParticleBase_b {
  public:


    inline static int counter = 0;
    T data;

    ParticleBase();
    ParticleBase(std::string name, T v);


};





template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase(std::string name, T v) : data(v){
    ++counter;
    bunch_ID = name;
    std::cout << "creating particle container" << std::endl;
    
    // Validate type before registering
    VisBase::validateParticleType<T, Dim>();
    
    VisBase::pb_c.push_back(this);
}


template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase() : 
    ParticleBase<T,Dim>("unnamed_"+ std::to_string(counter), 0)
{
}


