constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"
// Field mappings
REGDYN_REGISTER_NAME_TYPE("E", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho", Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi", Field<vec<double,1>, 1>);
REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);


int main(){
    
    VisAdaptorBase& visman(bpl::visman_g);   // defined in a .cpp
    bpl::visman_g.init_global();

    
    // Switch manager to the global registry so it sees all auto-registered fields
    VisAdaptorBase visman; // dynamic, name-only registry (starts with local)
    visman.init_global();

    

    // Auto-registered fields using compile-time fixed-string tags
    Field<vec<double,3>, 3> F1;                         // not registered (no tag)
    Field<vec<double,2>, 2> F2{ id<"rho"> };     // auto-registers as "rho"
    Field<vec<double,1>, 1> F3{ id<"phi"> };     // auto-registers as "phi"
    

    Field<double, 1> fE{ id<"E"> };              // auto-registers as "E"
    Field<double, 1> fdensity{ id<"density"> };  // auto-registers as "density"



    // Access directly without manual visman.add<...>(...)
    std::cout <<"fieee E"  << bpl::visman_g.get_registry().template Get<"E">().data << std::endl;
    std::cout <<"fieee E"  << bpl::visman_g.get_registry().Get<"E">().data << std::endl;
    std::cout <<"fieee E"  << bpl::visman_g.get_registry().Get(id<"E">).data << std::endl;
    
    
    
    
    std::cout <<"fieephi"  << bpl::visman_g.get_registry().Get<"phi">().data << std::endl;
    std::cout <<"fieerho"  << bpl::visman_g.get_registry().Get<"rho">().data << std::endl;
    std::cout << "density" << bpl::visman_g.get_registry().Get<"density">().data << std::endl;



    // Rebind example: create a new density field and it will overwrite the binding
    Field<double, 1> fdensity2{ id<"density"> }; // rebind on construction
    std::cout << "density(rebound)" << bpl::visman_g.get_registry().template Get<"density">().data << std::endl;


    return 0;
}





