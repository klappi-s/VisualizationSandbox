#pragma once
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>




template <typename T, unsigned Dim>
struct vec : public std::array<T, Dim> {
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
template<typename T, unsigned Dim, unsigned VDim>
class VisRegistry ;

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
    std::unique_ptr<VisBase> vb;void initializeVisualization() {bpl::vb = std::make_unique<VisBase>();}
}