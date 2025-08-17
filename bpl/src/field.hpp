#include"bpl.h"


template<typename T>
Field<T>::Field() : 
    Field<T>("unnamed_"+ std::to_string(counter), 0.0)
{
    ++counter;
}

template<typename T>
Field<T>::Field(std::string name, T v){


    using pc_t = ParticleBase<T>;
    using fc_t = Field<T>;

    fcName = name;
    value = v;

    assert( (VisManager<pc_t, fc_t>::fc ) && "VisManager<double>::fc is nullptr, no instance of VisManager was created" );
   
        std::cout << "creating field container" << std::endl;
        VisManager<pc_t, fc_t>::fc->push_back(this);
        
    
    
}


// VisBase<T>::pf = std::make_unique<std::vector<std::shared_ptr<Field<T>>>>();
// if (!VisBase<T>::fc) std::cout << "manager needs to be created befor" << std<::endl;
// VisBase<T>::fc->push_back(std::make_shared<Field<T>>());
// VisManager<pc_t, fc_t>::fc->push_back(std::make_unique<Field<T>>(*this));
// VisManager<pc_t, fc_t>::fc->push_back(std::make_shared<Field<T>>(*this));