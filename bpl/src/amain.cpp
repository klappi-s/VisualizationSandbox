#include "manager.h"





int main(){

    
    // if(!vb_t::fc){
    //     std::cout << "VisBase<double>::fc is nullptr_1" << std::endl;
    // } 


    BaseManagerVis manager;
    std::cout << "Manager created"<< std::endl;
    // !manager.vb.fc


    
    
    Field<double> f0;
    ParticleBase<double> p0;

    std::cout << vb_t::fc->size() << std::endl;
    std::cout << vb_t::pc->size() << std::endl;


    Field<double> f1("a Name", 3);
    // ParticleBase<double> p1(); not legal
    ParticleBase<double> p1;
    ParticleBase<double> p2("another Name");
    ParticleBase<double> p3;

    
    std::cout << vb_t::fc->size() << std::endl;
    std::cout << vb_t::pc->size() << std::endl;



    std::cout << (*vb_t::fc)[0]->fcName << "  value = " << (*vb_t::fc)[0]->value << std::endl;
    std::cout << (*vb_t::fc)[1]->fcName << "  value = " << (*vb_t::fc)[1]->value << std::endl;
    
    // std::cout << (*vb_t::pc)[0]->pcName << "  value = " << (*vb_t::pc)[0]->valuee << std::endl;
    std::cout << (*vb_t::pc)[0]->pcName << std::endl;
    std::cout << (*vb_t::pc)[1]->pcName << std::endl;
    std::cout << (*vb_t::pc)[2]->pcName << std::endl;
    std::cout << (*vb_t::pc)[3]->pcName << std::endl;
    
    
    
    


    // if(){
    //     std::cout << "nullptre2" << std::endl;
    // }
    // else{

    // }




    return 0;
}
