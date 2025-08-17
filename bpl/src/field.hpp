#include"manager.h"


template<typename T>
Field<T>::Field(){
    ++counter;
    if(counter < 10)
    Field<T>("unnamed_"+ std::to_string(counter), 0.0);
}

template<typename T>
Field<T>::Field(std::string name, T v){


    using pc_t = ParticleBase<T>;
    using fc_t = Field<T>;

    fcName = name;
    value = v;

    // VisBase<T>::pf = std::make_unique<std::vector<std::shared_ptr<Field<T>>>>();
    // if (!VisBase<T>::fc) std::cout << "manager needs to be created befor" << std<::endl;
    // VisBase<T>::fc->push_back(std::make_shared<Field<T>>());

    if(!VisBase<pc_t, fc_t>::fc){
        std::cout << "VisBase<double>::fc is nullptr" << std::endl;
    }
    else{
        std::cout << "creating field" << std::endl;
        VisBase<pc_t, fc_t>::fc->push_back(std::make_shared<Field<T>>(*this));
    }


}

