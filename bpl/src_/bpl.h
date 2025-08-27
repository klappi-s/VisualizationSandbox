#pragma once
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <typeindex>


#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

/* FORWARD DECLARATIONS */
template<typename T, unsigned Dim, unsigned VDim>
class VisRegistry;

class ParticleBase_b;

class Field_b;

template<typename T, unsigned Dim>
class ParticleBase;

template<typename T, unsigned Dim>
class Field;

#include "VisBase.h"
#include "particle.h"
#include "field.h"




namespace bpl {

    std::unique_ptr<VisBase> vb;
    
    void initializeVisualization() {
        bpl::vb = std::make_unique<VisBase>();
    }

    
}