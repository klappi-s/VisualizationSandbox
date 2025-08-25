


    // template <typename T, unsigned Dim>
    // using FieldVariant = std::variant<Field_t<Dim>*, VField_t<T, Dim>*>;

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <cxxabi.h>


constexpr unsigned Dim = 3;
using T                = double;

#include "bpl.h"




int main() {
    // // std::cout << estimate_total_containers<T,3>() << std::endl;
    
    // /* maybe show all works would be nice for versatility */
    // // Scalar field
    // // Vector field with std::array
    // // Vector field with custom vec class
    
    
    // // std::cout << "Scalar fields registered: " << vb_t::sf_c().get  ->size() << std::endl;
    // std::cout << "Scalar fields registered: " << vb_t::get_sf_c()->size() << std::endl;
    // std::cout << "ParticleBunches registered: " << vb_t::get_pb_c()->size() << std::endl;
    // /* core dump */
    // std::cout << "VF 3 3  registered: " << vb_t::get_vf_c<3,3>()->size() << std::endl;

    // Create fields - should see debug output
    Field<double, 1> scalarField1("temperaturasdasde", 25.0);
    Field<double, 2> scalarField2("temperatuasdare", 25.0);
    Field<double, 3> scalarField0("temperature", 25.0);
    Field<double, 3> scalarField3;
    Field<double, 3> scalarField4("namemeee");
    Field<vec<double, 3>, 3> vectorField1("velocity", {1.0, 2.0, 3.0});
    Field<vec<double, 3>, 3> vectorField2("force", {5.0, 10.0});
    Field<vec<double, 2>, 3> vectorField3("force23", vec<double, 2>{5.0});
    Field<vec<double, 2>, 3> vectorField4;
    Field<vec<double, 1>, 3> vectorField5;
    Field<vec<double, 2>, 2> vectorField6;

    
    
    // std::cout << "\n=== Testing Registration ===" << std::endl;

    using vb_t = VisRegistry<T, 3>;
    vb_t registry;
    
    
    std::cout << "Scalar fields registered: " << vb_t::get_sf_c()->size() << std::endl;
    std::cout << "ParticleBunches registered: " << vb_t::get_pb_c()->size() << std::endl;
    /* core dump */
    std::cout << "VF 3 3  registered: " << vb_t::get_vf_c<3,3>()->size() << std::endl;
    

    registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
        std::cout   << "Scalar field: " << field.field_ID 
                    << " (Dim=" << dim  << ")"
                    << " with data: " << field.data
                    << std::endl;             
    });
    


    // Then use it:
    registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
        std::cout << "Vector field: " << field.field_ID 
                  << " (Dim=" << dim << ", VDim=" << vdim << ")" 
                  << " with data: ";

                    std::cout << "[";
                    for (unsigned i = 0; i < vdim; ++i) {
                        if constexpr (requires { field.data[i]; }) {
                            std::cout << field. data[i];
                        } 
                        if (i < vdim - 1) std::cout << ", ";
                    }
                    std::cout << "]"<<std::endl;
    });


    
    std::cout << "1. Testing empty registry:\n";
    registry.print_state();
    





    return 0;
}



