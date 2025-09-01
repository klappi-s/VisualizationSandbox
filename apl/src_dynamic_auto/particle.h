#pragma once
#include "bpl.h"



class ParticleBase_b{
    public:
    std::string bunch_ID;
 
};

template<typename T, unsigned Dim>
class ParticleBase : public ParticleBase_b {
  public:


    // std::array<T, Dim> data;
    vec<T, Dim> data;

    ParticleBase();
    ParticleBase(std::string name);
    ParticleBase(std::string name, T v);


};





template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase(std::string name, T v) : data({v}){
    
    bunch_ID = name;
        std::cout << "creating particle container (valued)" << std::endl;
        
}



template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase(std::string name){
    
    bunch_ID = name;
    fill_with_random(data);
        std::cout << "creating particle container (named)" << std::endl;
        
}


template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase() 
{
        bunch_ID = "Particle<"+ std::string(typeid(T).name()).substr(0,1) + ","+std::to_string(Dim)+">_unlabeled";
        fill_with_random(data);
        std::cout << "creating particle container (default)" << std::endl;
}





