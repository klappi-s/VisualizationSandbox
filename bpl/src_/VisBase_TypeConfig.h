#pragma once

// Type configuration macros for VisBase
// These can be overridden by the user before including bpl.h

#ifndef BPL_SCALAR_TYPE
#define BPL_SCALAR_TYPE double
#endif

#ifndef BPL_DEFAULT_DIM
#define BPL_DEFAULT_DIM 3
#endif

// Alias the configured types for internal use
namespace bpl_internal {
    using ScalarType = BPL_SCALAR_TYPE;
    constexpr unsigned DefaultDim = BPL_DEFAULT_DIM;
}
