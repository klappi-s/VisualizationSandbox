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

    // Factory use to create VisAdaptor and underlying registry
    auto visman = MakeVisAdaptor<"E","rho","phi","density">(F1, F2, F3, f1);

    /* get registry and retrieve reference to data */
    auto& registry = visman.get_registry();
    std::cout << registry.Get<"E">().data << std::endl;
    std::cout << registry.Get<"phi">().data << std::endl;
    std::cout << registry.Get<"rho">().data << std::endl;
    std::cout << registry.Get<"density">().data << std::endl;
    


    /* can do everything directly from the VisAdaptor,
    not necessarily a must
    */

    /* Check registered objects */
    if (visman.contains<"density">()) {
        std::cout << "visman contains density" << std::endl;
    }
    
    /*  retrieve reference and access member via Visualisation Adaptor */
    std::cout << visman.get<"E">().data << std::endl;
    std::cout << visman.get<"phi">().data << std::endl;
    std::cout << visman.get<"rho">().data << std::endl;


    std::cout << "density" << visman.get<"density">().data << std::endl;

    /* overwrite data in a set field id */
    visman.set<"density">(f2);
    std::cout << "density" << visman.get<"density">().data << std::endl;
    std::cout << "f1" << f1.data << std::endl;
    std::cout << "f2" << f2.data << std::endl;
    
    

    
    /* creation of visAdapator from 
    unique or shared pointer  to registry ... */
    
    /* Factory to create  registry ... */
    auto registry_new = MakeRegistry<"E","rho">(F1, F2);
    auto visman_new = MakeVisAdaptor(registry_new);
    
    /*  retrieve reference and access member via Visualisation Adaptor */
    std::cout << visman_new.get<"E">().data << std::endl;
    std::cout << visman_new.get<"rho">().data << std::endl;
    

    return 0;
}





