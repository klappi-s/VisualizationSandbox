constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){
    // Fluent compile-time accumulation
    Field<double, 1> fE;
    Field<vec<double,2>, 2> frho;
    Field<vec<double,1>, 1> fphi;

    // Start with empty builder type
    

    Field<vec<double,3>, 3> F1;
    Field<vec<double,2>, 2> F2;
    Field<vec<double,1>, 1> F3;
    

    
    Field<double, 1> f1;
    Field<double, 1> f2;
    Field<double, 1> f3;

    // Extend adaptor slots to include "density" so it can be set/get later
    auto visman = MakeVisAdaptor<"E","rho","phi","density">(f3, F2, F3, f1);

    // VisAdaptorBase<> base;
    // Add returns a new type with accumulated slots; chain in a single statement
    auto vis = std::move(visman)
        .add<"E2">(fE)
        .add<"rho2">(frho)
        .add<"phi">(fphi);

    auto reg = vis.get_registry();
    
    std::cout << vis.get<"E">().data << std::endl;
    std::cout << vis.get(id<"E">).data << std::endl;  

    std::cout << reg.Get<"E">().data << std::endl;    
    std::cout << reg.Get(id<"E">).data << std::endl;  
    



    // Use compile-time gets
    std::cout << vis.get<"E2">().data << std::endl;
    std::cout << vis.template get<"rho">().data << std::endl;
    std::cout << vis.template get<"phi">().data << std::endl;



    // auto z = reg.Get(id<"E">);
    
    // auto z = reg->Get<"E2">();



    //     Field<double, 1> f1;
    // Field<double, 1> f2;

    VisAdaptorBase<> v0;
    auto v1 = std::move(v0).add<"density">(f1);

    std::cout << "density: " << v1.template get<"density">().data << "\n";

    // Adding another density would create a different compile-time slot; typically you use unique IDs
    auto v2 = std::move(v1); // just move, keep same slots
    v2.template set<"density">(f2); // rebind same slot
    std::cout << "density rebound: " << v2.template get<"density">().data << "\n";
    return 0;
}
