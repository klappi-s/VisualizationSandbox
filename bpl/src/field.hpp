#include"bpl.h"


template<typename T, unsigned Dim>
Field<T,Dim>::Field() : 
    Field<T,Dim>("unnamed_"+ std::to_string(counter), 0.0)
{
    ++counter;
}

template<typename T, unsigned Dim>
Field<T,Dim>::Field(std::string name, T v):
field_ID(name), value(v){


    assert( (VisRegistry<T, Dim>::sf_c ) && "VisRegistry<double>::sf_c is nullptr, no instance of VisRegistry was created" );
   
        std::cout << "creating field container" << std::endl;
        VisRegistry<T, Dim>::sf_c->push_back(this);
        
    
    
}
