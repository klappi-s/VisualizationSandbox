constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"
// Field mappings
REGDYN_REGISTER_NAME_TYPE("E", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho", Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi", Field<vec<double,1>, 1>);
REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);


int main(){

    Field<vec<double,3>, 3> F1;
    Field<vec<double,2>, 2> F2;
    Field<vec<double,1>, 1> F3;

    Field<double, 1> f1;
    Field<double, 1> f2;
    Field<double, 1> f3;

    // Construct adaptor with an explicit registry instance
    VisAdaptorBase visman(std::make_shared<RegistryDynamic>());

    // Bind entries (late binding allowed) via the registry
    visman.get_registry().Set<"E">(f1);
    visman.get_registry().Set<"rho">(F2);
    visman.get_registry().Set<"phi">(F3);
    visman.get_registry().Set<"density">(f1);

    std::cout << visman.get_registry().Get<"E">().data << std::endl;
    std::cout << visman.get_registry().Get<"phi">().data << std::endl;
    std::cout << visman.get_registry().Get<"rho">().data << std::endl;

    std::cout << "density" << visman.get_registry().Get<"density">().data << std::endl;

    // rebind
    visman.get_registry().Set<"density">(f2);
    std::cout << "f1" << f1.data << std::endl;
    std::cout << "f2" << f2.data << std::endl;

    std::cout << "density" << visman.get_registry().Get<"density">().data << std::endl;

    // Demonstrate reset with shared_ptr (warns because current registry is non-empty)
    std::cout << "-- reset with shared_ptr --\n";
    visman.reset_registry(std::make_shared<RegistryDynamic>());
    // Rebind after reset
    visman.get_registry().Set<"E">(f1);
    std::cout << visman.get_registry().Get<"E">().data << std::endl;

    // Demonstrate reset with unique_ptr (also warns if non-empty)
    std::cout << "-- reset with unique_ptr --\n";
    visman.reset_registry(std::make_unique<RegistryDynamic>());
    visman.get_registry().Set<"rho">(F2);
    std::cout << visman.get_registry().Get<"rho">().data << std::endl;

    // Demonstrate constructor with unique_ptr promoted to shared_ptr
    std::cout << "-- construct with unique_ptr --\n";
    VisAdaptorBase vis_uni(std::make_unique<RegistryDynamic>());
    vis_uni.get_registry().Set<"phi">(F3);
    std::cout << vis_uni.get_registry().Get<"phi">().data << std::endl;

    return 0;
}





