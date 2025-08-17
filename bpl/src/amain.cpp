#include "bpl.h"



    // template <typename T, unsigned Dim>
    // using FieldVariant = std::variant<Field_t<Dim>*, VField_t<T, Dim>*>;

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(){

    
    using pc_t_ = ParticleBase<double>;
    using fc_t_ = Field<double>;
    using vb_t = VisManager<pc_t_, fc_t_>;

    
    // Field<double> fff;
    // ParticleBase<double> ppp;
    // vb_t vm;
    // BaseManagerVis manager;
    

    bpl::initializeVisualization<pc_t_, fc_t_>();
    



    std::cout << "Manager created"<< std::endl;
    std::cout << vb_t::fc->size() << std::endl;
    Field<double> f0;
    std::cout << vb_t::fc->size() << std::endl;
    Field<double> f1("a Name", 3);
    std::cout << vb_t::fc->size() << std::endl;
    Field<double> f2("another Name", 3);
    Field<double> f3;
    std::cout << vb_t::fc->size() << std::endl;
    std::cout << (*vb_t::fc)[0]->fcName << " has  value = " << (*vb_t::fc)[1]->value << std::endl;
    std::cout << (*vb_t::fc)[1]->fcName << " has  value = " << (*vb_t::fc)[1]->value << std::endl;
    std::cout << (*vb_t::fc)[2]->fcName << " has  value = " << (*vb_t::fc)[1]->value << std::endl;
    std::cout << (*vb_t::fc)[3]->fcName << " has  value = " << (*vb_t::fc)[1]->value << std::endl;

    

    
    
    
    std::cout << vb_t::pc->size() << std::endl;
    ParticleBase<double> p0;
    ParticleBase<double> p1;
    ParticleBase<double> p2("P-Name", 1);
    ParticleBase<double> p3;
    std::cout << vb_t::pc->size() << std::endl;
    std::cout << (*vb_t::pc)[0]->pcName << std::endl;
    std::cout << (*vb_t::pc)[1]->pcName << std::endl;
    std::cout << (*vb_t::pc)[2]->pcName << std::endl;
    std::cout << (*vb_t::pc)[3]->pcName << std::endl;

    




    return 0;
}






    // std::vector<double&> pv;
    
    // auto a = std::vector<double>(); runs
    // auto a = std::vector<double&>();
    // auto a = std::make_shared<std::vector<double>>();
    

    // std::shared_ptr<std::vector<double&>> pp = std::make_shared<std::vector<double&>>();
    // std::shared_ptr<std::vector<double*>> pp = std::make_shared<std::vector<double*>>();
    
