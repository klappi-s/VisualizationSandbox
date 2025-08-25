#pragma once
#include <iostream>
#include <string>



class ParticleBase_b{
    public:
    std::string bunch_ID;
    bool is_registered = false;  // Track registration status
    
    // Check if this particle was successfully registered
    bool isRegistered() const { return is_registered; }
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
    is_registered = false;  // Initialize as not registered
    std::cout << "creating particle container" << std::endl;
    
    // Validate type before registering - catch exceptions internally
    try {
        VisBase::validateParticleType<T, Dim>();
        VisBase::pb_c.push_back(this);
        is_registered = true;  // Mark as successfully registered
    } catch (const std::runtime_error& e) {
        std::cout << "Particle creation failed: " << e.what() << std::endl;
        std::cout << "Particle '" << name << "' was not registered." << std::endl;
        // Particle object is created but not registered in the manager
    }
}


template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase() : 
    ParticleBase<T,Dim>("unnamed_"+ std::to_string(counter), 0)
{
}


