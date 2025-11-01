constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){


    {
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

    auto& reg = vis.get_registry();
    
    std::cout << reg.Get<"E">().data << std::endl;
    std::cout << reg.Get(id<"E">).data << std::endl;  

    std::cout << reg.Get<"E">().data << std::endl;    
    std::cout << reg.Get(id<"E">).data << std::endl;  
    



    // Use compile-time gets
    std::cout << reg.Get<"E2">().data << std::endl;
    std::cout << reg.Get(id<"rho">).data << std::endl;
    std::cout << reg.Get(id<"phi">).data << std::endl;



    // auto z = reg.Get(id<"E">);
    
    // auto z = reg->Get<"E2">();



    //     Field<double, 1> f1;
    // Field<double, 1> f2;

    VisAdaptorBase<> v0;
    auto v1 = std::move(v0).add<"density">(f1);

    std::cout << "density: " << v1.get_registry().template Get<"density">().data << "\n";

    // Adding another density would create a different compile-time slot; typically you use unique IDs
    auto v2 = std::move(v1); // just move, keep same slots
    v2.get_registry().template Set<"density">(f2); // rebind same slot
    std::cout << "density rebound: " << v2.get_registry().template Get<"density">().data << "\n";

    }




    // Demo: Rebind same variable name with VIS_REBIND
    {
        VisAdaptorBase<> vis;
        Field<double, 1> fA, fB;


        VIS_REBIND(vis, "density", fA){
        VIS_REBIND(vis, "phi", fB){

        std::cout << "VIS_REBIND density: " << vis.get_registry().Get(id<"density">).data << "\n";
        std::cout << "VIS_REBIND phi: " << vis.get_registry().Get(id<"phi">).data << "\n";
        
        } }
        
        
            
            
            
    }
        

    // // Demo: Rebind same variable name with VIS_REBIND
    // {
    //     VisAdaptorBase<> vis;
    //     Field<double, 1> fA, fB;


    //     VIS_REBIND(vis, "density", fA)
    //     VIS_REBIND(vis, "phi", fB)

    //     std::cout << "VIS_REBIND density: " << vis.get("density").data << "\n";
    //     std::cout << "VIS_REBIND phi: "     << vis.get("phi").data << "\n";
        
            
    // }
        
     

    // Demo: Continuation-style with_added helper
    {
        VisAdaptorBase<> vis;
        Field<double, 1> fx;
        with_added<"density">(std::move(vis), fx, [&](auto&& vis_after){
            std::cout << "with_added density: " << vis_after.get_registry().Get(id<"density">).data << "\n";
        });
    }













    return 0;
}
