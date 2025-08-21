#include"bpl.h"


template<typename T, unsigned Dim, unsigned VDim>
VisRegistry<T, Dim, VDim>::VisRegistry() {
    std::cout << "VisBase default constructor" << std::endl;
    
    
    if (!VisRegistry<T, Dim>::pb_c) {
        // VisRegistry<T, Dim>::pb_c = &pc_v;
        VisRegistry<T, Dim>::pb_c = std::make_unique<std::vector<pb_c_t*>>();
        // VisRegistry<T, Dim>::pb_c = std::make_unique<std::vector<std::unique_ptr<pc_T>>>();
        
    }
    
    
    if (!VisRegistry<T, Dim>::sf_c) {
        // VisRegistry<T, Dim>::sf_c = &fc_v;
        VisRegistry<T, Dim>::sf_c = std::make_unique<std::vector<sf_c_t*>>();
        // VisRegistry<T, Dim>::sf_c = std::make_unique<std::vector<std::unique_ptr<fc_T>>>();
    }
}


