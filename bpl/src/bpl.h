#pragma once



#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cassert>

#define assertm(exp, msg) assert((void(msg), exp))




// Forward declare ParticleBase to avoid circular include

// template<typename pc_T, typename fc_T>
// class VisBase;

template<typename pc_T, typename fc_T>
class VisManager ;
// : public VisBase;


/* FORWARD DECLARATIONS */
template<typename T>
class ParticleBase;

template<typename T>
class Field;



#include "VisBase.h"
#include "particle.h"
#include "field.h"


using pc_t_ = ParticleBase<double>;
using fc_t_ = Field<double>;
using vb_t = VisManager<pc_t_, fc_t_>;

// using vb_t_ = VisBase;

#include "manager.h"



namespace bpl{



    // std::shared_ptr<VisBase> ptr;
    // VisManager<pc_t_, fc_t_> vm;

    // ptr = std::make_shared(&)






}