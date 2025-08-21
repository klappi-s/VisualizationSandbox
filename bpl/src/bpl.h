#pragma once



#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>



/* FORWARD DECLARATIONS */

template<typename T, unsigned Dim, unsigned VDim>
class VisRegistry ;

template<typename T, unsigned Dim>
class ParticleBase;

template<typename T, unsigned Dim>
class Field;



#include "VisBase.h"
#include "particle.h"
#include "field.h"



namespace bpl {


        /* this works i guess ... */

        // std::unique_ptr<VisBase> vb;
        std::unique_ptr<VisRegistry<T, Dim>> vb;
        /* no need to be of visbase but important to see this works?*/



        template<typename T, unsigned Dim>
        void initializeVisualization() {
          bpl::vb = std::make_unique<VisRegistry<T, Dim>>();
        }




    // Default types... dont really work i thin for circumventing our problem ..
    // using DefaultParticle = ParticleBase<T,Dim>;
    // using DefaultField = Field<T,Dim>;
    // using DefaultVisRegistry = VisRegistry<DefaultParticle, DefaultField>;
}