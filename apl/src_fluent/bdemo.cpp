constexpr unsigned Dim = 3;
using T = double;

#include "bpl.h"

int main(){


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
