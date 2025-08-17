#pragma once



#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>



/* FORWARD DECLARATIONS */

template<typename pc_T, typename fc_T>
class VisManager ;

template<typename T>
class ParticleBase;

template<typename T>
class Field;



#include "VisBase.h"
#include "particle.h"
#include "field.h"
#include "manager.h"




namespace bpl {


        /* this works i guess ... */

        std::unique_ptr<VisBase> vb;

        template<typename pc_T, typename fc_T>
        void initializeVisualization() {
          bpl::vb = std::make_unique<VisManager<pc_T, fc_T>>();
        }




    // Default types... dont really work i thin for circumventing our problem ..
    // using DefaultParticle = ParticleBase<double>;
    // using DefaultField = Field<double>;
    // using DefaultVisManager = VisManager<DefaultParticle, DefaultField>;
}