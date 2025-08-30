constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){

    Field<vec<double,3>, 3> F1;
    Field<vec<double,2>, 2> F2;
    Field<vec<double,1>, 1> F3;
    

    
    Field<double, 1> f1;
    Field<double, 1> f2;
    Field<double, 1> f3;

    auto visman = MakeVisAdaptor<"E","rho","phi">(f1, F2,F3);
    
    
    // vec<int, 3> a({0,0,1});
    // std::cout << a  << std::endl;

    std::cout << visman.get_registry_entry<"E">().data << std::endl;
    std::cout << visman.get_registry_entry<"phi">().data << std::endl;
    std::cout << visman.get_registry_entry<"rho">().data << std::endl;

    



    
    return 0;
}





