#include"manager.h"


template<typename T>
ParticleBase<T>::ParticleBase(){
    ParticleBase<T>("unnamed_"+ std::to_string(counter));
    ++counter;
}

template<typename T>
ParticleBase<T>::ParticleBase(std::string name){


    using pc_t = ParticleBase<T>;
    using fc_t = Field<T>;


    pcName = name;

    // if (!VisBase<T>::pc) VisBase<T>::pc = std::make_unique<std::vector<std::shared_ptr<ParticleBase<T>>>>();


    if(!VisBase<pc_t, fc_t>::pc){
        std::cout << "VisBase<double>::pc is nullptr" << std::endl;
    }
    else{
        VisBase<pc_t, fc_t>::pc->push_back(std::make_shared<ParticleBase<T>>(*this));
    }

}
