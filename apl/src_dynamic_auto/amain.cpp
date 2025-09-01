constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"
// Field mappings
REGDYN_REGISTER_NAME_TYPE("E", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho", Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi", Field<vec<double,1>, 1>);
REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);


int main(){

    // Auto-registered fields using compile-time fixed-string tags
    Field<vec<double,3>, 3> F1;                         // not registered (no tag)
    Field<vec<double,2>, 2> F2{ fs_tag<"rho">{} };     // auto-registers as "rho"
    Field<vec<double,1>, 1> F3{ fs_tag<"phi">{} };     // auto-registers as "phi"
    

    Field<double, 1> fE{ fs_tag<"E">{} };              // auto-registers as "E"
    Field<double, 1> fdensity{ fs_tag<"density">{} };  // auto-registers as "density"

    VisAdaptorBase visman; // dynamic, name-only registry (starts with local)

    // Switch manager to the global registry so it sees all auto-registered fields
    

    // Access directly without manual visman.add<...>(...)
    std::cout <<"fieee E"  << bpl::visman_g.get_registry_entry<"E">().data << std::endl;
    std::cout <<"fieephi"  << bpl::visman_g.get_registry_entry<"phi">().data << std::endl;
    std::cout <<"fieerho"  << bpl::visman_g.get_registry_entry<"rho">().data << std::endl;
    std::cout << "density" << bpl::visman_g.get_registry_entry<"density">().data << std::endl;



    // Rebind example: create a new density field and it will overwrite the binding
    Field<double, 1> fdensity2{ fs_tag<"density">{} }; // rebind on construction
    std::cout << "density(rebound)" << bpl::visman_g.get_registry_entry<"density">().data << std::endl;


    return 0;
}





