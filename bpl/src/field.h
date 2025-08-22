#pragma once


#include <type_traits>
#include <array>




// Type trait to detect if T is a vector-like type
template<typename T>
struct is_vector_type : std::false_type {};

template<typename T2, unsigned VDim>
struct is_vector_type<std::array<T2, VDim>> : std::true_type {};

template<typename T2, unsigned VDim>
struct is_vector_type<vec<T2, VDim>> : std::true_type {};


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


// ===========================================================


// Field contaner template
template<typename T, unsigned Dim>
class Field{

        private:
        // Scalar floating point registration
        template<typename U = T>
        void registerField(std::integral_constant<int, 0>, 
            std::enable_if_t<std::is_floating_point_v<U>>* = nullptr) {

            // Fixed: Use ContainerManager for proper lazy initialization
            auto& container = ContainerManager<Field<T,Dim>>::get_container();
            std::cout << "Creating scalar field container" << std::endl;
            container->push_back(this);
        }

        // Vector type registration  
        template<typename U = T>
        void registerField(std::integral_constant<int, 0>,
                          std::enable_if_t<is_vector_type_v<U>>* = nullptr) {
            constexpr unsigned VDim = vector_dimension_v<U>;
            using ScalarType = scalar_type_t<U>;

            // Fixed: Use ContainerManager for vector fields
            auto& container = ContainerManager<Field<vec<ScalarType,VDim>,Dim>>::get_container();
            std::cout << "Creating vector field container (VDim=" << VDim << ")" << std::endl;
            container->push_back(this);
        }

        // Custom object registration
        template<typename U = T>
        void registerField(std::integral_constant<int, 0>,
                          std::enable_if_t<std::is_class_v<U> && !is_vector_type_v<U> && !std::is_floating_point_v<U>>* = nullptr) {

            assert(0 && "Invalid Class choice for T in class template Field<class T, unsigned Dim>");
        }



    
    public:
    inline static int counter = 0;
    std::string field_ID;
    T data;

    
    Field() :
    field_ID("Field<"+std::string(typeid(T).name())+","+std::to_string(Dim)+">_"
                + std::to_string(counter)+"_unlabeled"){
        registerField(std::integral_constant<int, 0>{});
        ++counter;
    }

    Field(std::string name) :
    field_ID(name){
        registerField(std::integral_constant<int, 0>{});
        ++counter;
    }

    Field(std::string name, T v) :
    field_ID(name), data(v) {
        registerField(std::integral_constant<int, 0>{});
        ++counter;
    }

};


#include"field.hpp"










/*

Basically implements:


template<typename T, unsigned Dim>
Field<T,Dim>::Field(std::string name, T v) : field_ID(name), value(v) {
    
    if constexpr (is_vector_type<T>::value) {
        // Vector field case: T = std::array<T2, VDim> or vec<T2, VDim>
        constexpr unsigned VDim = vector_dimension<T>::value;
        using ScalarType = scalar_type_t<T>;
        
        static_assert(std::is_floating_point_v<ScalarType>, 
                     "Vector field underlying type must be floating point");
        
        assert(VisSubRegistry<ScalarType, Dim, VDim>::vf_c && 
               "Vector field registry is nullptr");
        
        std::cout << "Creating vector field container (VDim=" << VDim << ")" << std::endl;
        VisSubRegistry<ScalarType, Dim, VDim>::vf_c->push_back(this);
        
    } else if constexpr (std::is_floating_point_v<T>) {
        // Scalar field case: T = double, float, etc.
        assert(VisSubRegistry<T, Dim, 1>::sf_c && 
               "Scalar field registry is nullptr");
        
        std::cout << "Creating scalar field container" << std::endl;
        VisSubRegistry<T, Dim, 1>::sf_c->push_back(this);
        
    } else {
        // Object/class template case
        static_assert(std::is_class_v<T>, "T must be a class type");
        
        // You'll need to define how to handle custom class types
        // For now, treat as scalar field
        assert(VisSubRegistry<T, Dim, 1>::sf_c && 
               "Object field registry is nullptr");
        
        std::cout << "Creating object field container" << std::endl;
        VisSubRegistry<T, Dim, 1>::sf_c->push_back(this);
    }
}






int main() {
    using vb_t = VisRegistry<double, 3>;
    vb_t registry;
    
    // Scalar field
    Field<double, 3> scalarField("temperature", 25.0);
    
    // Vector field with std::array
    Field<std::array<double, 3>, 3> vectorField1("velocity", {1.0, 2.0, 3.0});
    
    // Vector field with custom vec class
    Field<vec<double, 2>, 3> vectorField2("force", vec<double, 2>{5.0, 10.0});
    
    // Custom object field
    class CustomData {
    public:
        double mass;
        int id;
        CustomData(double m, int i) : mass(m), id(i) {}
    };
    Field<CustomData, 3> objectField("custom", CustomData{1.5, 42});
    
    // Count fields by type
    registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
        std::cout << "Scalar field: " << field.field_ID << std::endl;
    });
    
    registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
        std::cout << "Vector field: " << field.field_ID 
                  << " (Dim=" << dim << ", VDim=" << vdim << ")" << std::endl;
    });
    
    return 0;
}

*/