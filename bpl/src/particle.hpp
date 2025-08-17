#include"bpl.h"


template<typename T>
ParticleBase<T>::ParticleBase() : 
    ParticleBase<T>("unnamed_"+ std::to_string(counter), 0)
{
    ++counter;
}


template<typename T>
ParticleBase<T>::ParticleBase(std::string name, T v){


    using pc_t = ParticleBase<T>;
    using fc_t = Field<T>;


    pcName = name;
    values.push_back(v);
    
    assert( (VisManager<pc_t, fc_t>::pc ) && "VisManager<double>::pc is nullptr, no instance of VisManager was created" );
    
    
    std::cout << "creating particle container" << std::endl;
    VisManager<pc_t, fc_t>::pc->push_back(this);
    
}





// if (!VisBase<T>::pc) VisBase<T>::pc = std::make_unique<std::vector<std::shared_ptr<ParticleBase<T>>>>();
// VisManager<pc_t, fc_t>::pc->push_back(std::make_shared<ParticleBase<T>>(*this));