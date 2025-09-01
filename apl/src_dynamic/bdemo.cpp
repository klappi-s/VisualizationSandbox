constexpr unsigned Dim = 3;
using T = double;
#include "bpl.h"

// Field mappings
REGDYN_REGISTER_NAME_TYPE("E", Field<double, 1>);
REGDYN_REGISTER_NAME_TYPE("rho", Field<vec<double,2>, 2>);
REGDYN_REGISTER_NAME_TYPE("phi", Field<vec<double,1>, 1>);
REGDYN_REGISTER_NAME_TYPE("density", Field<double, 1>);


int main(){
    VisAdaptorBase vis(std::make_shared<RegistryDynamic>());

    // Runtime string API demo
    int score = 42;
    float health = 100.0f;

    vis.get_registry().add_named("score", score);
    vis.get_registry().add_named("health", health);

    if (auto* pScore = vis.get_registry().get_named<int>("score")) {
        std::cout << "score=" << *pScore << "\n";
    }
    if (auto* pHealth = vis.get_registry().get_named<float>("health")) {
        std::cout << "health=" << *pHealth << "\n";
    }

    // Compile-time API demo (uses registered IDs)
    Field<double, 1> density;
    vis.get_registry().Set<"density">(density);
    std::cout << "density bound: " << vis.get_registry().Contains<"density">() << "\n";
    vis.get_registry().Unset<"density">();
    std::cout << "density after remove: " << vis.get_registry().Contains<"density">() << "\n";

    return 0;
}




