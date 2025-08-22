#include"bpl.h"


template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase() : 
    ParticleBase<T,Dim>("unnamed_"+ std::to_string(counter), std::array<T,Dim>{})
{
    ++counter;
}


template<typename T, unsigned Dim>
ParticleBase<T,Dim>::ParticleBase(std::string name, std::array<T,3> v){

    bunch_ID = name;
    values.push_back(v);
    
    assert( (VisSubRegistry<T,Dim,1>::pb_c ) && "VisRegistry<double>::pb_c is nullptr, no instance of VisRegistry was created" );  
    
    std::cout << "creating particle container" << std::endl;
    VisSubRegistry<T, Dim,1>::pb_c->push_back(this);
    
}

