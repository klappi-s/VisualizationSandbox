#pragma once



#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


template <typename T, unsigned Dim>
struct vec : public std::array<T, Dim> {
    // Inherit constructors from std::array
    // using std::array<T, Dim>::array;

    // // Named accessors (const and non-const versions)
    // // These functions are only available if Dim is large enough.

    T& x() requires (Dim >= 1) { return (*this)[0]; }
    const T& x() const requires (Dim >= 1) { return (*this)[0]; }

    T& y() requires (Dim >= 2) { return (*this)[1]; }
    const T& y() const requires (Dim >= 2) { return (*this)[1]; }

    T& z() requires (Dim >= 3) { return (*this)[2]; }
    const T& z() const requires (Dim >= 3) { return (*this)[2]; }

    T& w() requires (Dim >= 4) { return (*this)[3]; }
    const T& w() const requires (Dim >= 4) { return (*this)[3]; }
};


/* FORWARD DECLARATIONS */

// template<typename T, unsigned Dim, unsigned VDim>
// class VisRegistry ;

template<typename T, unsigned Dim>
class ParticleBase;

template<typename T, unsigned Dim>
class Field;



#include "VisBase.h"
#include "VisBase.hpp"
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