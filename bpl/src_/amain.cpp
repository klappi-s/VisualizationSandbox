


    // template <typename T, unsigned Dim>
    // using FieldVariant = std::variant<Field_t<Dim>*, VField_t<T, Dim>*>;

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>


constexpr unsigned Dim = 3;
using T           = double;

#include "bpl.h"

int main(){

    
    bpl::initializeVisualization();
    std::cout << "Manager created"<< std::endl;
    

    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

    
    ParticleBase<T,Dim> p0;
    Field<T,Dim> f0;
    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

        Field<T,Dim> f1("a Name", 1.0);
    Field<T,Dim> f2("another Name", 2.0);
    
    ParticleBase<T,Dim> p1;
    ParticleBase<T,Dim> p2("P-Name", 2);
    
    // Create a vector field for testing
    Field<vec<double,3>, 2> vfield("vector_field", {{1.0, 2.0, 3.0}});
    Field<vec<double,3>, 2> vfield2;
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;
    



    std::cout << "\n.\n.\n.\n.\n" << std::endl;


    // Demonstrate field retrieval by ID
    std::cout << "\n=== Field Retrieval Demo ===" << std::endl;
    VisBase::listAllFields();
    
    // Find and print info for a specific field
    std::cout << "\n=== Field 'a Name' Info ===" << std::endl;
    VisBase::printFieldInfo("a Name");
    
    std::cout << "\n======" << std::endl;    // Retrieve data from field
    std::any data = VisBase::getFieldData("a Name");
    if (data.has_value()) {
        try {
            double value = std::any_cast<double>(data);
            std::cout << "Retrieved value: " << value << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "Failed to cast data: " << e.what() << std::endl;
        }
    }

    // Test vector field retrieval
    std::cout << "\n=== Vector Field 'vector_field' Data ===" << std::endl;
    std::any vec_data = VisBase::getFieldData("vector_field");
    if (vec_data.has_value()) {
        try {
            auto vec_value = std::any_cast<vec<double, 3>>(vec_data);
            std::cout << "Retrieved vector: (" << vec_value[0] << ", " 
                      << vec_value[1] << ", " << vec_value[2] << ")" << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "Failed to cast vector data: " << e.what() << std::endl;
        }
    }






    
    // // Field<double,Dim> f3;
    // // ParticleBase<T,2> p3;
    
    
    
    // std::cout << bpl::vb->get_size_pb() << std::endl;
    // std::cout << bpl::vb->get_size_sf() << std::endl;


    
    
    // // Field<vec<double,3>,Dim> Vfield;


    // std::cout << bpl::vb->get_size_pb() << std::endl;
    // std::cout << bpl::vb->get_size_sf() << std::endl;


    
    
    
    
   // std::cout << bpl::vb->sf_c[0]->field_ID << " has  value = " <<bpl::vb->sf_c[0]->data << std::endl;
    // std::cout << bpl::vb->pb_c[0]->bunch_ID << "has Value ...." << std::endl;

    

    return 0;
}

