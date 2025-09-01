#pragma once

#include "Vis_forward.h"
#include "VisRegistry.h"
#include "VisBase.h"
#include "VisBase.hpp"


#include "particle.h"
#include "field.h"





// Define bpl namespace variables and functions
// namespace bpl {

    // std::unique_ptr<VisBase> vb;
    // void initializeVisualization() {
    //     bpl::vb = std::make_unique<VisBase>();
    // }
namespace bpl {
    RegistryDynamic reg_g; // NOLINT(cert-err58-cpp)
    VisAdaptorBase visman_g;
    visman.init_global();
    // dynamic, name-only registry (defined in a .cpp)
}


