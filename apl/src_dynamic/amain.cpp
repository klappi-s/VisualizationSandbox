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

    VisAdaptorBase visman; // dynamic, name-only registry

    // Bind entries (late binding allowed)
    visman.add<"E">(f1);
    visman.add<"rho">(F2);
    visman.add<"phi">(F3);
    visman.add<"density">(f1);

    std::cout << visman.get<"E">().data << std::endl;
    std::cout << visman.get<"phi">().data << std::endl;
    std::cout << visman.get<"rho">().data << std::endl;

    std::cout << "density" << visman.get<"density">().data << std::endl;

    visman.add<"density">(f2); // rebind
    std::cout << "f1" << f1.data << std::endl;
    std::cout << "f2" << f2.data << std::endl;

    std::cout << "density" << visman.get<"density">().data << std::endl;

    return 0;
}





