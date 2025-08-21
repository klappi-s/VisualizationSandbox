


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

    
    // using pc_t_ = ParticleBase<T,Dim>;
    // using fc_t_ = Field<T,Dim>;
    using vb_t = VisRegistry<T,Dim>;

    
    // Field<T,Dim> fff;
    // ParticleBase<T,Dim> ppp;
    // vb_t vm;
    // BaseManagerVis manager;
    

    bpl::initializeVisualization<T, Dim>();
    



    std::cout << "Manager created"<< std::endl;
    std::cout << vb_t::sf_c->size() << std::endl;
    Field<T,Dim> f0;
    std::cout << vb_t::sf_c->size() << std::endl;
    Field<T,Dim> f1("a Name", 1.0);
    std::cout << vb_t::sf_c->size() << std::endl;
    Field<T,Dim> f2("another Name", 2.0);
    Field<T,Dim> f3;


    std::cout << vb_t::sf_c->size() << std::endl;
    std::cout << (*vb_t::sf_c)[0]->field_ID << " has  value = " << (*vb_t::sf_c)[0]->value << std::endl;
    std::cout << (*vb_t::sf_c)[1]->field_ID << " has  value = " << (*vb_t::sf_c)[1]->value << std::endl;
    std::cout << (*vb_t::sf_c)[2]->field_ID << " has  value = " << (*vb_t::sf_c)[2]->value << std::endl;
    std::cout << (*vb_t::sf_c)[3]->field_ID << " has  value = " << (*vb_t::sf_c)[3]->value << std::endl;

    

    
    
    
    std::cout << vb_t::pb_c->size() << std::endl;
    ParticleBase<T,Dim> p0;
    ParticleBase<T,Dim> p1;
    ParticleBase<T,Dim> p2("P-Name", std::array<T,Dim>{1.0,2,3});
    ParticleBase<T,Dim> p3;
    std::cout << vb_t::pb_c->size() << std::endl;
    std::cout << (*vb_t::pb_c)[0]->bunch_ID << std::endl;
    std::cout << (*vb_t::pb_c)[1]->bunch_ID << std::endl;
    std::cout << (*vb_t::pb_c)[2]->bunch_ID << std::endl;
    std::cout << (*vb_t::pb_c)[3]->bunch_ID << std::endl;

    




    return 0;
}






    // std::vector<double&> pv;
    
    // auto a = std::vector<double>(); runs
    // auto a = std::vector<double&>();
    // auto a = std::make_shared<std::vector<double>>();
    

    // std::shared_ptr<std::vector<double&>> pp = std::make_shared<std::vector<double&>>();
    // std::shared_ptr<std::vector<double*>> pp = std::make_shared<std::vector<double*>>();
    
