// Enhanced VisBase.h with user-friendly auto-compatible API
#pragma once

#include <any>
#include <array>
#include <cstddef>


#include <cassert>
#include <functional>

#include <memory>
#include <iostream>

#include <optional>
#include <random> 

#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <type_traits>

#include <vector>

#include <unordered_map>
#include <unordered_set>


#include <iostream>
#include <cstddef>
#include <stdexcept>



#include <string>
#include <unordered_map>
#include <stdexcept>

// Compile-time string literal wrapper (C++20 NTTP) used as IDs.
template <std::size_t N>
struct fixed_string {
    char data[N]{};
    constexpr fixed_string(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) data[i] = str[i];
    }
    constexpr std::string_view sv() const { return std::string_view{data, N - 1}; }
};

template <std::size_t N, std::size_t M>
constexpr bool operator==(const fixed_string<N>& a, const fixed_string<M>& b) {
    if constexpr (N != M) return false;
    for (std::size_t i = 0; i < N; ++i) if (a.data[i] != b.data[i]) return false;
    return true;
}

// Helper tag to pass fixed_string IDs as a type. Usage: id<"name">
template <fixed_string Id>
struct id_tag { static constexpr auto value = Id; };

template <fixed_string Id>
inline constexpr id_tag<Id> id{};



// vec template definition (needed for vector field types)
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

// Stream output operator for vec
template <typename T, unsigned Dim>
std::ostream& operator<<(std::ostream& os, const vec<T, Dim>& v) {
    os << "[";
    for (unsigned i = 0; i < Dim; ++i) {
        os << v[i];
        if (i < Dim - 1) os << ", ";
    }
    os << "]";
    return os;
}




template<typename T, std::size_t ROWS>
void fill_with_random(std::array<T, ROWS>& arr) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
                  "Type T must be an integral or floating-point type.");

    std::random_device rd;
    std::mt19937 gen(rd());
    
    T min = 0;
    T max = 9;

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> distrib(min, max);
        for (auto& row : arr) {
                row = distrib(gen);
            
        }
    } 
    else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> distrib(min, max);
        for (auto& row : arr) {
                row = distrib(gen);
            
        }
    }
}



template<typename T, std::size_t ROWS, unsigned COLS>
void fill_with_random(std::array<vec<T, COLS>, ROWS>& arr) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
                  "Type T must be an integral or floating-point type.");

    std::random_device rd;
    std::mt19937 gen(rd());
    
    T min = 0;
    T max = 9;

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> distrib(min, max);
        for (auto& row : arr) {
            for (unsigned i = 0; i < COLS; ++i) {
                row[i] = distrib(gen);
            }
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> distrib(min, max);
        for (auto& row : arr) {
            for (unsigned i = 0; i < COLS; ++i) {
                row[i] = distrib(gen);
            }
        }
    }
}


// Generic print function to check our work
template<typename T, std::size_t ROWS, unsigned COLS>
void print_2d_array(std::array<vec<T, COLS>, ROWS>& arr) {
    // Set precision for floating-point types for cleaner output
    for (const auto& row : arr) {
        for (const auto& element : row) {
            std::cout << element << '\t';
        }
        std::cout << '\n';
    }
     std::cout << std::defaultfloat; // Reset cout format
}




// template <typename T, unsigned Dim>
// struct vec : public std::array<T, Dim>;

// Type trait to detect if T is a vector-like type
template<typename T>
struct is_vector_type : std::false_type {};

template<typename T2, unsigned VDim>
struct is_vector_type<std::array<T2, VDim>> : std::true_type {};

template<typename T2, unsigned VDim>
struct is_vector_type<vec<T2, VDim>> : std::true_type {};



// Forward declare vec to avoid circular dependency
template<typename T, unsigned Dim>
struct vec;

// Helper template to detect vec types
template<typename T>
struct is_vec : std::false_type {};

template<typename ScalarType, unsigned VDim>
struct is_vec<vec<ScalarType, VDim>> : std::true_type {
    using scalar_type = ScalarType;
    static constexpr unsigned vdim = VDim;
};








// Extract VDim from vector types
template<typename T>
struct vector_dimension {
    static constexpr unsigned value = 1; // Default for scalar types
};

template<typename T2, unsigned VDim>
struct vector_dimension<std::array<T2, VDim>> {
    static constexpr unsigned value = VDim;
};

template<typename T2, unsigned VDim>
struct vector_dimension<vec<T2, VDim>> {
    static constexpr unsigned value = VDim;
};

// Extract underlying scalar type
template<typename T>
struct scalar_type {
    using type = T; // Default for scalar types
};

template<typename T2, unsigned VDim>
struct scalar_type<std::array<T2, VDim>> {
    using type = T2;
};

template<typename T2, unsigned VDim>
struct scalar_type<vec<T2, VDim>> {
    using type = T2;
};

template<typename T>
using scalar_type_t = typename scalar_type<T>::type;

// Add variable template shortcuts for our custom traits
template<typename T>
constexpr bool is_vector_type_v = is_vector_type<T>::value;

template<typename T>
constexpr unsigned vector_dimension_v = vector_dimension<T>::value;





// Forward declarations updated for dynamic registry API
class VisAdaptorBase; // was: template <typename... Slots> class VisAdaptorBase;

class RegistryBase;

class RegistryDynamic;


template<typename T, unsigned Dim>
class ParticleBase;

template<typename T, unsigned Dim>
class Field;

