// #include "VisBase.h"
// #include "VisBase.hpp"
// #include "field.h"
// #include "particle.h"
// #include "bpl.h"
// #include <iostream>
// #include <algorithm>


// // Define bpl namespace variables and functions
// namespace bpl {
//     std::unique_ptr<VisBase> vb;
    
//     void initializeVisualization() {
//         bpl::vb = std::make_unique<VisBase>();
//     }
// }




// // Find scalar field by ID
// Field_b* VisBase::findScalarFieldByID(const std::string& field_id) {
//     for (Field_b* field : sf_c) {
//         if (field && field->field_ID == field_id) {
//             return field;
//         }
//     }
//     return nullptr;
// }


// // List all fields (both scalar and vector)
// void VisBase::listAllFields() {
//     std::cout << "\n=== All Registered Fields ===" << std::endl;
    
//     int total_fields = sf_c.size() + vf_c.size();
//     if (total_fields == 0) {
//         std::cout << "No fields registered." << std::endl;
//         return;
//     }
    
//     listAllScalarFields();
//     listAllVectorFields();
// }

// // List scalar fields
// void VisBase::listAllScalarFields() {
//     std::cout << "\n=== Registered Scalar Fields ===" << std::endl;
//     if (sf_c.empty()) {
//         std::cout << "No scalar fields registered." << std::endl;
//         return;
//     }
    
//     for (size_t i = 0; i < sf_c.size(); ++i) {
//         if (sf_c[i]) {
//             std::cout << "S" << i << ": " << sf_c[i]->field_ID << std::endl;
//         }
//     }
// }
