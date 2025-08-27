


constexpr unsigned Dim = 3;
using T           = double;



#include "VIS_forward.h"

#include "bpl.h"





int main(){


    bpl::initializeVisualization();
    std::cout << "Manager created"<< std::endl;
    

    


    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

    
    // ParticleBase<T,Dim> p0;
    // Field<T,Dim> f0;
    
    
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;

    Field<T,Dim> f1("FA_Name",  1.0);
    Field<T,Dim> f2("FB_FName", 2.0);
    
    // ParticleBase<T,Dim> p1;
    // ParticleBase<T,Dim> p2("PA_Name", 2);
    
    Field<vec<double,3>, 2> vfielda("VA_Name");
    Field<vec<double,2>, 3> vfieldb("VB_Name");
    Field<vec<double,1>, 1> vfieldc("VC_Name");
    Field<vec<double,3>, 2> vfieldd;
    
    
    
    
    /* PRINT INFO FUNCTION */
    std::cout << bpl::vb->get_size_pb() << std::endl;
    std::cout << bpl::vb->get_size_sf() << std::endl;
    
    
    std::cout << "\n" << std::endl;
    std::cout << "\n=== VisBase::listAllFields();    Field Retrieval Demo ===" << std::endl;
    VisBase::PrintListFields();
    


    std::cout << "\n" << std::endl;
    std::cout << "\nVisBase::printFieldInfo('F...')" << std::endl;
    
    VisBase::printFieldInfo("FA_Name");
    VisBase::printFieldInfo("VA_Name");
    VisBase::printFieldInfo("VB_Name");
    VisBase::printFieldInfo("VC_Name");
    

    


    // Demonstrate field retrieval by ID
    std::cout << "\nVisBase::findFieldByID('VA_Name')" << std::endl;
    
    std::cout << VisBase::findFieldByID("VA_Name") <<std::endl;
    std::cout << VisBase::findFieldByID("VB_Name") <<std::endl;
    std::cout << VisBase::findFieldByID("VC_Name") <<std::endl;

    // VisBase::printFieldInfo("PA_Name");
    





    /*  get reference for void and base class functions ... */
    /* base class functions can be accessed directly  */
    // std::cout << "\n======" << std::endl;    // Retrieve data from field
    // std::any data = VisBase::getFieldData("a Name");

    // if (data.has_value()) {
    //     try {
    //         double value = std::any_cast<double>(data);
    //         std::cout << "Retrieved value: " << value << std::endl;
    //     } catch (const std::bad_any_cast& e) {
    //         std::cout << "Failed to cast data: " << e.what() << std::endl;
    //     }
    // }

    // // Test vector field retrieval
    // std::cout << "\n=== Vector Field 'vector_field' Data ===" << std::endl;
    // std::any vec_data = VisBase::getFieldData("vector_field");
    // if (vec_data.has_value()) {
    //     try {
    //         auto vec_value = std::any_cast<vec<double, 3>>(vec_data);
    //         std::cout << "Retrieved vector: (" << vec_value[0] << ", " 
    //                   << vec_value[1] << ", " << vec_value[2] << ")" << std::endl;
    //     } catch (const std::bad_any_cast& e) {
    //         std::cout << "Failed to cast vector data: " << e.what() << std::endl;
    //     }
    // }



    return 0;
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

    


