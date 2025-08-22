


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

    // using vb_t = VisRegistry<T, 3>;
    // vb_t registry;
    
    
    // std::cout << "Scalar fields registered: " << vb_t::get_sf_c()->size() << std::endl;
    // std::cout << "ParticleBunches registered: " << vb_t::get_pb_c()->size() << std::endl;
    // /* core dump */
    // std::cout << "VF 3 3  registered: " << vb_t::get_vf_c<3,3>()->size() << std::endl;
    

    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
    //     std::cout   << "Scalar field: " << field.field_ID 
    //                 << " (Dim=" << dim  << ")"
    //                 << " with data: " << field.data
    //                 << std::endl;             
    // });
    


    // // Then use it:
    // registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
    //     std::cout << "Vector field: " << field.field_ID 
    //               << " (Dim=" << dim << ", VDim=" << vdim << ")" 
    //               << " with data: ";

    //                 std::cout << "[";
    //                 for (unsigned i = 0; i < vdim; ++i) {
    //                     if constexpr (requires { field.data[i]; }) {
    //                         std::cout << field. data[i];
    //                     } 
    //                     if (i < vdim - 1) std::cout << ", ";
    //                 }
    //                 std::cout << "]"<<std::endl;
    // });


    VisRegistry<double, 3> registry;
    
    std::cout << "1. Testing empty registry:\n";
    registry.print_state();
    





    return 0;
}








    // // Test field modification
    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) noexcept {
    //     field.data *= 2.0;  // Fixed: 'data' not 'value'
    //     std::cout << "Modified field " << field.field_ID 
    //               << " at Dim " << dim << " new value: " << field.data << std::endl;
    // });
    







    



    // // Count fields by type
    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
    //     std::cout << "Scalar field: " << field.field_ID << std::endl;
    // });
    
    // registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
    //     std::cout << "Vector field: " << field.field_ID 
    //               << " (Dim=" << dim << ", VDim=" << vdim << ")" << std::endl;
    // });



    
    // // Performance: Add noexcept to lambdas when possible
    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) noexcept {
    //     field.value *= 2.0;
    //     std::cout << "Field at Dim " << dim << std::endl;
    // });
    
    // // Access containers same as before
    // auto& scalar_fields = VisRegistry3D::sf_c();
    // auto& vector_fields = VisRegistry3D::vf_c<3, 2>();
    
    








// int main3() {
//     using vb_t = VisRegistry<T, Dim>;
//     vb_t registry;
    
    // // Apply to all scalar fields across all dimensions
    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
    //     std::cout << "Scalar field at Dim " << dim << " has value: " << field.value << std::endl;
    //     field.value *= 2.0;  // Example operation
    // });
    
    // // Apply to all particle bunches across all dimensions
    // registry.apply_to_all_particle_bunches([](auto& particle, unsigned dim) {
    //     std::cout << "Particle bunch at Dim " << dim << " ID: " << particle.bunch_ID << std::endl;
    //     // Example: move all particles
    //     for (auto& pos : particle.position) {
    //         pos += 0.1;
    //     }
    // });
    
    // // Apply to all vector fields across the 2D grid
    // registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
    //     std::cout << "Vector field at (" << dim << "," << vdim << ")" << std::endl;
    // });
    
    
    // Count total items
//     size_t total_scalar_fields = 0, total_particles = 0, total_vector_fields = 0;
    
//     registry.apply_to_all_scalar_fields([&](auto& field, unsigned dim) {
//         total_scalar_fields++;
//     });
    
//     registry.apply_to_all_particle_bunches([&](auto& particle, unsigned dim) {
//         total_particles++;
//     });
    
//     registry.apply_to_all_vector_fields([&](auto& field, unsigned dim, unsigned vdim) {
//         total_vector_fields++;
//     });
    
//     std::cout << "Total: " << total_scalar_fields << " scalar fields, "
//               << total_particles << " particle bunches, "
//               << total_vector_fields << " vector fields" << std::endl;
    
//     return 0;
// }
     
    //   Call member functions simply like:
    // registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
    //     // Call field methods directly
    //     field.setValue(5.0);  // If you have a setValue method
    //     field.updateTimestamp();  // If you have an updateTimestamp method
    //     std::cout << "Field " << field.getID() << " at Dim " << dim 
    //               << " has value: " << field.getValue() << std::endl;
    // });
     






// --- Main Function for Demonstration ---
// /* gemini */
// int main() {
//     using vb_t = VisRegistry<double, 3>;
//     vb_t registry; // This object's construction ensures the static pointers are initialized.

//     // --- Create and Register a Scalar Field ---
//     Field<double, 3> scalarField("temperature", 25.0);
//     // vb_t::get_sf_c<3>()->push_back(&scalarField);
    
//     Field<double, 2> scalarField2("pressure", 101.3);
//     // vb_t::get_sf_c<2>()->push_back(&scalarField2);


//     // --- Create and Register Vector Fields ---
//     Field<vec<double, 3>, 3> vectorField1("velocity", {1.0, 2.0, 3.0});
//     // vb_t::get_vf_c<3, 3>()->push_back(&vectorField1);

//     Field<vec<double, 2>, 3> vectorField2("force", {5.0, 10.0});
//     // vb_t::get_vf_c<3, 2>()->push_back(&vectorField2);
    
//     Field<vec<double, 2>, 2> vectorField3("acceleration", {9.8, 0.0});
//     // vb_t::get_vf_c<2, 2>()->push_back(&vectorField3);


//     // --- Apply the functions to the registered fields ---
//     std::cout << "--- Applying to Scalar Fields ---" << std::endl;
//     registry.apply_to_all_scalar_fields([](auto& field, unsigned dim) {
//         std::cout << "Scalar field: " << field.field_ID << " (Dim=" << dim << ")" << std::endl;
//     });

//     std::cout << "\n--- Applying to Vector Fields ---" << std::endl;
//     registry.apply_to_all_vector_fields([](auto& field, unsigned dim, unsigned vdim) {
//         std::cout << "Vector field: " << field.field_ID
//                   << " (Dim=" << dim << ", VDim=" << vdim << ")" << std::endl;
//     });

//     return 0;
// }








 

    // // Custom object field
    // class CustomData {
    // public:
    //     double mass;
    //     int id;
    //     CustomData(double m, int i) : mass(m), id(i) {}
    // };
    // Field<CustomData, 3> objectField("custom", CustomData{1.5, 42});
    












// int main2(){

    
    // using pc_t_ = ParticleBase<T,Dim>;
    // using fc_t_ = Field<T,Dim>;
    // using vb_t = VisRegistry<T,Dim>;

    
    // Field<T,Dim> fff;
    // ParticleBase<T,Dim> ppp;
    // vb_t vm;
    // BaseManagerVis manager;
    

    // bpl::initializeVisualization<T, Dim>();
    



    // std::cout << "Manager created"<< std::endl;
    // std::cout << vb_t::sf_c->size() << std::endl;
    // Field<T,Dim> f0;
    // std::cout << vb_t::sf_c->size() << std::endl;
    // Field<T,Dim> f1("a Name", 1.0);
    // std::cout << vb_t::sf_c->size() << std::endl;
    // Field<T,Dim> f2("another Name", 2.0);
    // Field<T,Dim> f3;


    // std::cout << vb_t::sf_c->size() << std::endl;
    // std::cout << (*vb_t::sf_c)[0]->field_ID << " has  value = " << (*vb_t::sf_c)[0]->value << std::endl;
    // std::cout << (*vb_t::sf_c)[1]->field_ID << " has  value = " << (*vb_t::sf_c)[1]->value << std::endl;
    // std::cout << (*vb_t::sf_c)[2]->field_ID << " has  value = " << (*vb_t::sf_c)[2]->value << std::endl;
    // std::cout << (*vb_t::sf_c)[3]->field_ID << " has  value = " << (*vb_t::sf_c)[3]->value << std::endl;

    

    
    
    
    // std::cout << vb_t::pb_c->size() << std::endl;
    // ParticleBase<T,Dim> p0;
    // ParticleBase<T,Dim> p1;
    // ParticleBase<T,Dim> p2("P-Name", std::array<T,Dim>{1.0,2,3});
    // ParticleBase<T,Dim> p3;
    // std::cout << vb_t::pb_c->size() << std::endl;
    // std::cout << (*vb_t::pb_c)[0]->bunch_ID << std::endl;
    // std::cout << (*vb_t::pb_c)[1]->bunch_ID << std::endl;
    // std::cout << (*vb_t::pb_c)[2]->bunch_ID << std::endl;
    // std::cout << (*vb_t::pb_c)[3]->bunch_ID << std::endl;

    




//     return 0;
// }






    // std::vector<double&> pv;
    
    // auto a = std::vector<double>(); runs
    // auto a = std::vector<double&>();
    // auto a = std::make_shared<std::vector<double>>();
    

    // std::shared_ptr<std::vector<double&>> pp = std::make_shared<std::vector<double&>>();
    // std::shared_ptr<std::vector<double*>> pp = std::make_shared<std::vector<double*>>();
    
