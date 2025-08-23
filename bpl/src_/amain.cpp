


    // template <typename T, unsigned Dim>
    // using FieldVariant = std::variant<Field_t<Dim>*, VField_t<T, Dim>*>;

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>


constexpr unsigned Dim = 3;
using T                = double;

#include "bpl.h"

int main(){

    
    bpl::initializeVisualization();
    std::cout << "Manager created"<< std::endl;
    

    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

    
    ParticleBase<T,Dim> p0;
    Field<T,Dim> f0;
    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

    

    Field<T,Dim> f1("a Name", 1.0);
    Field<T,Dim> f2("another Name", 2.0);
    
    ParticleBase<T,Dim> p1;
    ParticleBase<T,Dim> p2("P-Name", 2);
    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;
    
    
    
    Field<float,Dim> f3;
    ParticleBase<T,2> p3;
    
    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;


    
    
    // Field<vec<double,3>,Dim> Vfield;


    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;


    
    
    
    
   // std::cout << bpl::vb->sf_c[0]->field_ID << " has  value = " <<bpl::vb->sf_c[0]->data << std::endl;
    // std::cout << bpl::vb->pb_c[0]->bunch_ID << "has Value ...." << std::endl;

    

    return 0;
}

